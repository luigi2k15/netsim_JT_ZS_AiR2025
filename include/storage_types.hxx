#ifndef storage_types
#define storage_types
#include "package.hxx"
enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        using const_iterator = std::list<Package>::const_iterator;
        virtual void push(Package&& pacz) = 0;
        virtual const_iterator end() = 0;
        virtual const_iterator begin() = 0;
        virtual const_iterator cend() = 0;
        virtual const_iterator cbegin() = 0;
        virtual size_t size() = 0;
        virtual bool empty() = 0;
        virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
    public:
        ~IPackageQueue() override = default;
        virtual Package pop() = 0;
};

class PackageQueue : public IPackageQueue {
    public:
        PackageQueue(PackageQueueType typ) : typ_(typ) {}
        Package pop()
        {
            if(typ_ == PackageQueueType::LIFO)
            {
                Package buf = kolejka_.back();
                kolejka_.pop_back();
                return buf;
            }
            else if(typ_ == PackageQueueType::FIFO)
            {
                Package buf = kolejka_.front();
                kolejka_.pop_front();
                return buf;
            }
            Package buf = kolejka_.back();
            return buf;
        }
        void push(Package&& pacz)
        {
            this->kolejka_.emplace_back(std::move(pacz));
        }
        const_iterator end() override
        {
            return this->kolejka_.end();
        }
        const_iterator begin() override
        {
            return this->kolejka_.begin();
        }
        const_iterator cend() override
        {
            return this->kolejka_.cend();
        }
        const_iterator cbegin() override
        {
            return this->kolejka_.cbegin();
        }
        size_t size() override
        {
            return this->kolejka_.size();
        }
        bool empty() override
        {
            return this->kolejka_.empty();
        }
        ~PackageQueue() override = default;
    private:
        std::list<Package> kolejka_;
        PackageQueueType typ_;
};
#endif
