#include<mutex>

struct some_big_object {};

void swap(some_big_object& lhs, some_big_object& rhs);

class X {
private:
	some_big_object some_detail;
	std::mutex m;

public:
	X(some_big_object const& sd) : some_detail(sd) {}

	friend void old_swap(X& lhs, X& rhs)
	{
		// in c++14
		if (&lhs == &rhs)
		{
			return;
		}

		// this avoids deadlocks
		std::lock(lhs.m, rhs.m);
		std::lock_guard lock_l(lhs.m, std::adopt_lock);
		std::lock_guard lock_r(rhs.m, std::adopt_lock);
		swap(lhs.some_detail, rhs.some_detail);
	}

	friend void swap(X& lhs, X& rhs)
	{
		// in C++17
		if (&lhs == &rhs)
		{
			return;
		}

		std::scoped_lock lock(lhs.m, rhs.m);
		swap(lhs.some_detail, rhs.some_detail);
	}
};