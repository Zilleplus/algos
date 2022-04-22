#include<mutex>
#include<algorithm>
#include <queue>
#include <condition_variable>

struct data_chunk {};

bool more_data_to_pepare()
{
	return false;
}

data_chunk prepare_data()
{
	return {};
}

void process(const data_chunk& d)
{}

bool is_last_chunk(const data_chunk& d)
{
	return true;
}

std::mutex mut;
std::queue<data_chunk> data_queu;
std::condition_variable data_cond;

void data_preparation_thread()
{
	while (more_data_to_pepare())
	{
		const data_chunk data = prepare_data();
		{
			std::lock_guard<std::mutex> lk(mut);
			data_queu.push(data);
		}
		data_cond.notify_one();
	}
}

void data_processing_thread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [] {return !data_queu.empty(); });
		data_chunk data = data_queu.front();
		data_queu.pop();
		lk.unlock();
		process(data);
		if (is_last_chunk(data))
		{
			break;
		}
	}
}