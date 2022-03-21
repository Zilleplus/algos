#include<exception>
#include<memory>
#include<mutex>
#include<stack>

struct empty_stack : std::exception {
};

template<typename T>
class threadsafe_stack {
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard l(m);
		data = other.data;
	}

	// No assignment operation
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_value)
	{
		std::lock_guard l(m);
		data.push(std::move(new_value));
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard l(m);
		if (std::empty(data)) { throw empty_stack{}; }
		// copy before returnig, if it throws here, the stack is still fine.
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	void pop(T& value)
	{
		std::lock_guard l(m);
		if (std::empty(data)) { throw empty_stack{}; }
		value = data.top();
		data.pop();
	}

	bool empty() const
	{
		std::lock_guard l(m);
		return std::empty(data);
	}
};