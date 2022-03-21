#include<mutex>
#include<memory>

struct some_resource {
	void do_something() {}
};
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void foo_3_11()
{
	std::unique_lock lk(resource_mutex);
	// If we move the lock inside the if loop,
	// we get a race condition on read/write of 
	// resource_ptr.
	if (!resource_ptr)
	{
		resource_ptr.reset(new some_resource);
	}
	lk.unlock();
	resource_ptr->do_something();
}