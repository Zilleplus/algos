#include<thread>
#include<iostream>
#include<string>
#include<syncstream>
#include<chrono>// to get the sleep
#include<sstream>
#include<utility>


struct background_task {
	void operator()()
	{
		std::cout << "Executing background task";
	}
};

class thread_guard {
	std::thread& t;
public:
	explicit thread_guard(std::thread& t_):
		t(t_){}

	thread_guard(const thread_guard&) = delete;

	thread_guard operator=(const thread_guard&) = delete;

	~thread_guard()
	{
		if(t.joinable())
		{
			t.join();
		}
	}
};

void ThreadManagement()
{
	std::cout << "ThreadManagement" << std::endl;
	background_task f;
	std::thread t(f);
	// you have to join or detach the thread
	// if you do neither, and the program ends before the thread ends.
	// the program will do std::terminate!
	// visual studio will fail an assert in the destructor 
	// of std::thread so you get some warning.
	t.join();
	// t.detach();

	// "C++ most vexing parsing" -> be careful here:
	// std::thread IAmAFunctionPoint(background_task());
	// some solutions:
	// -> std::thread IAmNotAFunctionPoint{ background_task() };
	// -> std::thread IAmAlsoNotAFunctionPoint( background_task{} );

	std::thread t2(background_task{});
	thread_guard tg(t2);
}

void f(int i, const std::string& s)
{
	std::stringstream ss;
	ss << i << ": " << s[20] << std::endl;
	const auto msg = ss.str();

    std::osyncstream sync_cout(std::cout);
	sync_cout << msg;
}

struct NotCopyableObject {
	NotCopyableObject () {}
	NotCopyableObject (const NotCopyableObject &) = delete;
};
void f_with_not_movable(const NotCopyableObject& obj)
{}

void PassingArgumentsToThread()
{
	using namespace std::chrono_literals;

	std::cout << "PassingArgumentsToThread" << std::endl;

	char buffer[1024];
	buffer[20] = 'A';

	// If we just pass it in like this, 
	// it will only work if we don't terminate
	// before the thread is at the end.
	std::thread t1(f, 1, buffer);
	t1.detach();
	
	// This copies the char array into a string object
	// the arguments for the function are variadic,
	// and in this case resolved to pass by value.
	// so no ownershipe problem here
	std::thread t2(f, 2, std::string(buffer));
	t2.detach();

	NotCopyableObject obj;
	// This won't compile as you can't do a pass by reference
	// it's alway's copied. (which kinda makes sense here).
	// std::thread t3(f_with_not_movable, obj);
	// wrap it in a std::ref, and then it will work.
	// there is however the lifetime problem again.
	std::thread t3(f_with_not_movable, std::ref(obj));
	t3.join();

	std::this_thread::sleep_for(2s);
}

class scoped_thread {
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) :t(std::move(t_)) {
		if(!t.joinable())
		{
			throw std::logic_error("No thread");
		}
	}

	~scoped_thread()
	{
		// We have ownership, so alway's join.
		t.join();
	}

	// no copy or assign
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;

};

void f_wait()
{
	// do nothing
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(2s);
}
void TransferOwnershipOfThread()
{
	std::thread t(f_wait);
	scoped_thread sss( std::move(t) );

	// This thread will join by itself,
	// unless you call the detach.
	// C++ 20 feature
	std::jthread t_joining(f_wait);
	// t_joining.detach(); // if you want to detach-> won't join when leaving scope.
}

void WaitAndPrintThreadId()
{
	f_wait();
	std::stringstream ss;
	ss << "The id of the thread from the inside is: " << std::this_thread::get_id() << std::endl;
	auto msg = ss.str();

    std::osyncstream sync_cout(std::cout);
	sync_cout << msg;
}

void IdentifyingThreads()
{
	std::jthread t1(WaitAndPrintThreadId);

	std::stringstream ss;
	ss << "The id of the thread from the outside is: " << t1.get_id() << std::endl;
	auto msg = ss.str();

    std::osyncstream sync_cout(std::cout);
	sync_cout << msg;
	
}

int main(int argc, int* argv)
{
	ThreadManagement();
	PassingArgumentsToThread();
	TransferOwnershipOfThread();
	IdentifyingThreads();
}