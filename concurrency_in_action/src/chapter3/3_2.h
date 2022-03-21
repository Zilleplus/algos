#include<string>
#include<mutex>

class some_data {
	int a;
	std::string b;
public:
	void do_something();
};

class data_wrapper {
private:
	some_data data;
	std::mutex m;

public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard l(m);
		func(data);
	}
};
