#include"queue.h"

namespace chapter4 {
	int pq_parent(int n)
	{
		if (n == 1)
		{
			return -1;
		}

		return static_cast<int>(n / 2);
	}

	int pq_young_child(int n) {
		return (2 * n);
	}

	void pq_insert(PriorityQueue& q, int p)
	{
		q.push_back(p);
		bubble_up(q, std::size(q) - 1);
	}

	void bubble_up(PriorityQueue& q, int position)
	{
		if (pq_parent(position) == -1)
		{
			return; // at root of heap, no parent
		}

		if (q[pq_parent(position)] > q[position])
		{
			std::swap(q[pq_parent(position)], q[position]);
			bubble_up(q, pq_parent(position));
		}
	}
}