#include"4_5_thread_safe_queue.h"
#include<syncstream>
#include<iostream>

void demo_thread_safe_queue()
{
	std::osyncstream sync_cout(std::cout);

	threadsafe_queue<int> q;
	sync_cout << "pushing 0" << std::endl;
	q.push(0);
	sync_cout << "pushing 1" << std::endl;
	q.push(1);
	sync_cout << "pushing 2" << std::endl;
	q.push(2);

	auto first_pop = q.try_pop();
	sync_cout << "popped: " << *first_pop << std::endl;

	auto second_pop = q.try_pop();
	sync_cout << "popped: " << *second_pop << std::endl;

	auto third_pop = q.try_pop();
	sync_cout << "popped: " << *third_pop << std::endl;
}