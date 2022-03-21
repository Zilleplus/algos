#include"3_2.h"
#include <iostream>
#include <syncstream>

void some_data::do_something()
{
	std::osyncstream sync_cout(std::cout);
	sync_cout << "doing some stuff";

}

some_data* unprotected;
void malicious_function(some_data& protected_data)
{
	unprotected = &protected_data;
}

data_wrapper x;
void foo()
{
	x.process_data(malicious_function);
	// You can call the do something on the internal data of
	// x while the lock is already release.
	unprotected->do_something();
}
