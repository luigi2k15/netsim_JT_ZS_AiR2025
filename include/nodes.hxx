#ifndef nodes
#define nodes
#include "helpers.cpp"
#include "package.hxx"
#include "storage_types.hxx"
//#include<bits/stdc++.h>
#include <optional>
#include <memory>
#include <iostream>

enum class ReceiverType{
    WORKER,
    STOREHOUSE
};

class IPackageReceiver {
    public:
        virtual ElementID get_id() const = 0;
        virtual void receive_package(Package&& p) = 0;
        virtual IPackageStockpile::const_iterator end() = 0;
        virtual IPackageStockpile::const_iterator begin() = 0;
        virtual IPackageStockpile::const_iterator cend() = 0;
        virtual IPackageStockpile::const_iterator cbegin() = 0;
        virtual ReceiverType get_typp() = 0;
        virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
    public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;
        ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator) : pg_(std::move(pg)) {}
        void add_receiver(IPackageReceiver* r)
        {
            preferences_t_[r];
            update_probabilities();
        }
        void remove_receiver(IPackageReceiver* r)
        {
            preferences_t_.erase(r);
            update_probabilities();
        }
        void update_probabilities()
        {
            double equal_prob = 1.0 / preferences_t_.size();
            for (auto& pair : preferences_t_)
            {
                pair.second = equal_prob;
                std::cout << equal_prob << std::endl;
            }
        }
        const_iterator end()
        {
            return preferences_t_.end();
        }
        const_iterator begin()
        {
            return preferences_t_.begin();
        }
        const_iterator cend()
        {
            return preferences_t_.cend();
        }
        const_iterator cbegin()
        {
            return preferences_t_.cbegin();
        }
        IPackageReceiver* choose_receiver()
        {
            double losowe = pg_();
            double suma = 0.0;
            for (const auto& pair : preferences_t_)
            {
                suma += pair.second;
                if (losowe <= suma)
                {
                    return pair.first;
                }
            }
            return preferences_t_.rbegin()->first;
        }
        const preferences_t& get_preferences() const {return this->preferences_t_;}
    private:
        preferences_t preferences_t_;
        ProbabilityGenerator pg_;
};

class PackageSender {
    public:
        PackageSender() = default;
        PackageSender(PackageSender&& mov) = default;
        ReceiverPreferences receiver_preferences_;
        void send_package()
        {
            if(!buff_.has_value())
            {
                return;
            }
            IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
            receiver->receive_package(std::move(*buff_));
            buff_.reset();
            return;
        }
        void push_package(Package&& moved)
        {
            buff_.emplace(moved.get_id());
            return;
        }
        std::optional<Package>& get_sending_buffer()
        {
            return buff_;
        }
    protected:
        std::optional<Package> buff_ = std::nullopt;
};

class Worker : public IPackageReceiver, PackageSender {
    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)) {}
        ElementID get_id() const override {return id_;}
        TimeOffset get_processing_duration() const {return pd_;}
        Time get_package_processing_start_time() const {return st_;}
        ReceiverType get_typp() override
        {
            return ReceiverType::WORKER;
        }
        void do_work(Time t)
        {

            if(!buff_.has_value() && !q_->empty())
            {
                ct_ = t;
                buff_.emplace(q_->pop());
            }
            if(t-ct_ == pd_)
            {
                push_package(Package(buff_.value().get_id()));
                buff_.reset();
            }
        }
        void receive_package(Package&& p) override
        {
            q_->push(std::move(p));
        }
        void send_package()
        {
            buff_ = std::nullopt;
            return;
        }
        std::optional<Package>& get_sending_buffer()
        {
            return buff_;
        }
        IPackageStockpile::const_iterator end() override
        {
            return q_->end();
        }
        IPackageStockpile::const_iterator begin() override
        {
            return q_->begin();
        }
        IPackageStockpile::const_iterator cend() override
        {
            return q_->cend();
        }
        IPackageStockpile::const_iterator cbegin() override
        {
            return q_->cbegin();
        }
        IPackageQueue* get_queue() const
        {
            return q_.get();
        }

        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;
        Worker(Worker&&) = default;
        Worker& operator=(Worker&&) = default;
        ~Worker() = default;
    private:
        Time st_, ct_= 0; //st_ - start time, ct_ - current time
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<PackageQueue> q_;
};

class Ramp : public PackageSender {
    public:
        Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
        ElementID get_id() const {return id_;}
        TimeOffset get_delivery_interval() const {return di_;}
        void deliver_goods(Time t)
        {
            if(t%di_ == 0)
            {
                buff_ = Package();
            }
        }
        void send_package()
        {
            buff_ = std::nullopt;
            return;
        }
        std::optional<Package>& get_sending_buffer()
        {
            return buff_;
        }
        Ramp(Ramp&&) = default;
    private:
        ElementID id_;
        TimeOffset di_;
        std::unique_ptr<PackageQueue> q_;
        std::optional<Package> buff_ = std::nullopt;
};

class Storehouse : public IPackageReceiver {
    public:
        Storehouse(ElementID id, std::unique_ptr<PackageQueue> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {}
        ElementID get_id() const override {return id_;}
        ReceiverType get_typp() override
        {
            return ReceiverType::STOREHOUSE;
        }
        void receive_package(Package&& p) override
        {
            d_->push(std::move(p));
        }
        IPackageStockpile::const_iterator end() override
        {
            return d_->end();
        }
        IPackageStockpile::const_iterator begin() override
        {
            return d_->begin();
        }
        IPackageStockpile::const_iterator cend() override
        {
            return d_->cend();
        }
        IPackageStockpile::const_iterator cbegin() override
        {
            return d_->cbegin();
        }
    private:
        ElementID id_;
        std::unique_ptr<PackageQueue> d_;
};

#endif
