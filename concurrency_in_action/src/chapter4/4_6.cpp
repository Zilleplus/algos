#include<future>
#include<iostream>

int some_expensive_computation()
{
	return 0;
}

void main_4_6()
{
	std::future<int> answer = std::async(some_expensive_computation);
	std::cout << "the answer is: " << answer.get();
}