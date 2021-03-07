#include<list>
#include<iterator>
#include<iostream>

auto Sort(std::list<int>& nums)
{
    std::list<int> sorted_nums;
    while(std::size(nums) > 0)
    {
        auto min_element = nums.begin();
        for(auto cursor = nums.begin(); cursor!=nums.end(); std::advance(cursor, 1))
        {
            if(*min_element > *cursor)
            {
                min_element = cursor;
            }
        }
        sorted_nums.push_back(*min_element);
        nums.erase(min_element);
    }
    return sorted_nums;
}

void print(std::list<int>& l)
{
    std::cout << "[ ";
    for(auto e : l)
    {
        std::cout << e << " ";
    }
    std::cout << "]";

    std::cout << std::endl;
}

int main()
{
    std::list<int> nums = {3, 7, 1, 10, 20, 15,30, 100};

    print(nums);
    auto sorted = Sort(nums);
    print(nums);
    print(sorted);
}
