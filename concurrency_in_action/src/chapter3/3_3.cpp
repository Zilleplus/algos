#include<deque>
#include<stack>
#include<iostream>

template<typename T, typename Container = std::deque<T>>
class stack {
public:
	explicit stack(const Container&);
	explicit stack(const Container&&);
	template<typename Alloc> explicit stack(const Alloc);
	template<typename Alloc> stack(const Container&, const Alloc);
	template<typename Alloc> stack(Container&&, const Alloc&);
	template<typename Alloc> stack(stack&&, const Alloc&);

	bool empty() const;
	size_t size() const;
	T& top();
	T const& top() const;
	void push(T const&);
	void push(T&&);
	void pop();
	void swap(stack&&);
	template<typename... Args> void emplace(Args&&... args);
};

// problem with size/empty
std::stack<int> s;
void problem()
{
	if (!s.empty())
	{
		// Just before this, an other thread could have emptied 
		// the stack. -> race condition
		int const value = s.top();
		std::cout << "problem() called s.top()=" << value << "\n";

		s.pop();
		// do_something(value);
	}
}