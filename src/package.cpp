#include "package.hxx"
#include <bits/stdc++.h>

int main()
{
    if(1)
    {
        Package paczka1;
        std::cout << paczka1.get_id() << std::endl;
        Package paczka2;
        std::cout << paczka2.get_id() << std::endl;
        Package paczka3 = Package(2);
        std::cout << paczka3.get_id() << std::endl;
        Package paczka4;
        std::cout << paczka4.get_id() << std::endl;
        Package paczka5;
        std::cout << paczka5.get_id() << std::endl;
        paczka5 = paczka4;
        std::cout << paczka5.get_id() << std::endl;
    }
    Package paczka6;
    std::cout << paczka6.get_id() << std::endl;
    return 0;
}
