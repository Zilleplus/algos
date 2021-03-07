#include<iostream>

int fib(int n)
{
    if(n<2)
    {
        return 1;
    }

    return fib(n-1) + fib(n-2);
}

int main()
{
    std::cout << "the fib of n="<< 5 << " is " << fib(5);
}
