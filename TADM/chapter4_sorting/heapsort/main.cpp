#include<iostream>
#include"queue.h"

using namespace chapter4;

void Print(PriorityQueue& pq)
{
	std::cout << "TODO";
}

int main()
{
	PriorityQueue p = {5,4,3,2,1,8,9};

	Print(p);
	std::cout << "bubble_up(p,0)" << std::endl;
	bubble_up(p, 0);
	Print(p);

	return 0;
}