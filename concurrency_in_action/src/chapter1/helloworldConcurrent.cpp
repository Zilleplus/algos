#include<iostream>
#include<thread>

void hello()
{
    std::cout << "Hello world";
}

int main()
{
    std::thread t(hello);
    t.join();
}
