#include"3_8_hierarchical_mutex.h"
#include<mutex>

hierarchical_mutex higher_level_mutex(10000);
hierarchical_mutex lower_level_mutex(5000);
hierarchical_mutex other_mutex(6000);

int do_low_level_stuff()
{
	return 0;
}

int low_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(lower_level_mutex);
	return do_low_level_stuff();
}

int do_high_level_stuff(int i)
{
	return 0;
}

int high_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(higher_level_mutex);
	return do_high_level_stuff(low_level_func());
}

void do_other_stuff() {}
void other_stuff()
{
	high_level_func();
	do_other_stuff();
}

void thread_b()
{
	// This violates the rules! -> runtime error
	// !!! you can't lock from a lower layer !!!
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
	other_stuff();
}
