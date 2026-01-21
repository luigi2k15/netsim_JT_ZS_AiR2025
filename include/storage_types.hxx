#ifndef storage_types
#define storage_types
#include "package.hxx"
enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    /*public:
        using std::list<Package>::const_iterator = const_iterator;
        virtual void push() = 0;
        virtual void end() = 0;
        virtual void begin() = 0;
        virtual void
        virtual void size() = 0;
        virtual void empty() = 0;
        virtual ~IPackageStockpile() {}*/
};

class PackageQueue {
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
        }
        void push(Package pacz)
        {
            kolejka_.push_back(pacz);
        }
        //void end() = 0;
        //void begin() = 0;
        int size() const
        {
            return kolejka_.size();
        }
        bool empty() const
        {
            return kolejka_.empty();
        }
    private:
        std::list<Package> kolejka_;
        PackageQueueType typ_;
};
#endif
