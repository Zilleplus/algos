#include<list>
#include<mutex>
#include<algorithm>

static std::list<int> some_list;
static std::mutex some_mutex;

void add_to_list(int new_value)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	const auto val = std::find(some_list.begin(), some_list.end(), value_to_find);
	return val != some_list.end();
}