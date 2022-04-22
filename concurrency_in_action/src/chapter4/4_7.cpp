#include<string>
#include<future>

struct X {
	void foo(int, std::string const&) {}
	std::string bar(std::string const& s) { return "called bar with " + s; }
};

X x;


struct Y {
	double operator()(double d)
	{
		return d * 2;
	}
};

class move_only {
public:
	move_only() {}
	move_only(move_only&&) noexcept {}
	move_only(move_only const&) = delete;
	move_only& operator=(move_only&&) noexcept {}
	void operator()() {}
};

X baz(X& x) { return x; }

void demo_4_7()
{
	auto f1 = std::async(&X::foo, &x, 42, "hello foo"); // pass by value of pointer -> x->foot("...")
	auto f2 = std::async(&X::bar, x, "hello bar"); // pass by value x
	Y y;
	auto f3 = std::async(Y(), 3.141); // call move ctor of y
	auto f4 = std::async(std::ref(y), 2.718); // pass by reference

	// in the book they did not use return type, msvc seems to require it
	auto nothing = std::async(baz, std::ref(x));
	nothing.wait();

	auto f5 = std::async(move_only());

	auto f6 = std::async(std::launch::async, Y(), 1.2);
	auto f7 = std::async(std::launch::deferred, baz, std::ref(x));
	auto f8 = std::async(std::launch::deferred | std::launch::async, baz, std::ref(x));
	auto f9 = std::async(baz, std::ref(x));
	f7.wait();
}


