#include<vector>

namespace chapter4
{
	using PriorityQueue  = std::vector<int>;

	int pq_parent(int n);

	int pq_young_child(int n);

	void bubble_up(PriorityQueue& q, int x);

	void pq_insert(PriorityQueue& q, int x);
}