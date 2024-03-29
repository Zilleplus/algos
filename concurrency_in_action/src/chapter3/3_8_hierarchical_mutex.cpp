#include"3_8_hierarchical_mutex.h"
#include<climits>
void hierarchical_mutex::check_for_hierarchy_violation() {
	if (this_thread_hierarch_value <= hierarchy_value)
	{
		throw std::logic_error("mutex hierarchy violated");
	}
}

void hierarchical_mutex::update_hierarchy_value() {
	previous_hierarchy_value = this_thread_hierarch_value;
	this_thread_hierarch_value = hierarchy_value;
}

hierarchical_mutex::hierarchical_mutex(unsigned long value) :
	hierarchy_value(value),
	previous_hierarchy_value(0) {
}

void hierarchical_mutex::lock() {
	check_for_hierarchy_violation(); // can throw
	internal_mutex.lock();
	update_hierarchy_value();
}

void hierarchical_mutex::unlock() {
	// don't do out of order unlocking,
	if (this_thread_hierarch_value != hierarchy_value)
	{
		throw std::logic_error("mutex hierarchy voilated -> might be out of order unlocking");
	}
	this_thread_hierarch_value = previous_hierarchy_value;
	internal_mutex.unlock();
}

bool hierarchical_mutex::try_lock() {
	check_for_hierarchy_violation();
	if (!internal_mutex.try_lock())
	{
		return false;
	}
	update_hierarchy_value();
	return true;
}

thread_local unsigned long hierarchical_mutex::this_thread_hierarch_value(ULONG_MAX);
