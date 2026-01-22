#include "nodes.hxx"
#include "storage_types.hxx"
#include "package.hxx"
#include <bits/stdc++.h>

int main()
{
    //std::unique_ptr<PackageQueue> qq = ;
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker w2(2, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker w3(2, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Ramp r(1, 2);
    Time t = 1;
    w.receive_package(Package(1));
    r.deliver_goods(t);
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    r.deliver_goods(t);
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();
    auto& buffer2 = r.get_sending_buffer();
    bool a = buffer.has_value();
    bool a2 = buffer2.has_value();
    std::cout << a << std::endl;
    std::cout << a2 << std::endl;
    int i = buffer.value().get_id();
    int i2 = buffer2.value().get_id();
    std::cout << i << std::endl;
    std::cout << i2 << std::endl;
    ReceiverPreferences prefs;
    prefs.add_receiver(&w);
    prefs.add_receiver(&w2);
    prefs.add_receiver(&w3);
    std::cout << prefs.choose_receiver()->get_id() << std::endl;
    return 0;
}
