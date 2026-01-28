#include "storage_types.hxx"

int main()
{
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));
    std::cout << q.size() << std::endl;
    Package p(std::move(q.pop()));
    return 0;
}
