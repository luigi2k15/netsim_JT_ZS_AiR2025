#ifndef nodes
#define nodes
#include "package.hxx"
#include "storage_types.hxx"
#include "helpers.hxx"
#include<bits/stdc++.h>
#include <optional>

class IPackageReceiver {
    public:
        virtual ElementID get_id() const = 0;
        virtual void receive_package(Package&& p) = 0;
        virtual ~IPackageReceiver() {}
};

class ReceiverPreferences {
    public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;
        ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator) : pg_(pg) {}
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
        const preferences_t& get_preferences() const {return preferences_t_;}
    private:
        preferences_t preferences_t_;
        ProbabilityGenerator pg_;
};

class Worker : public IPackageReceiver {
    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)) {}
        ElementID get_id() const override {return id_;}
        TimeOffset get_processing_duration() const {return pd_;}
        Time get_package_processing_start_time() const {return st_;}
        void do_work(Time t)
        {
            ct_ = t;
            if(tw_ == 0)
            {
                st_ = ct_;
                buff_ = q_->pop();
            }
            tw_++;
        }
        void receive_package(Package&& p) override
        {
            q_->push(p);
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
        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;
        Worker(Worker&&) = default;
        Worker& operator=(Worker&&) = default;
        ~Worker() = default;
    private:
        Time st_, ct_, tw_ = 0; //st_ - start time, ct_ - current time, tw_ - time worked
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<PackageQueue> q_;
        std::optional<Package> buff_ = std::nullopt;
};

class Ramp {
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
        Storehouse(ElementID id, std::unique_ptr<PackageQueue> d) : id_(id), d_(std::move(d)) {}
        ElementID get_id() const override {return id_;}
        void receive_package(Package&& p) override
        {
            d_->push(p);
        }
    private:
        ElementID id_;
        std::unique_ptr<PackageQueue> d_;
};

class PackageSender {

};

#endif
