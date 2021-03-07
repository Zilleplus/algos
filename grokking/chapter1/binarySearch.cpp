#include <iostream>
#include <optional>
#include <string>
#include <vector>

std::optional<int> BinarySearch(const std::vector<int>& nums, int element)
{
    int left = 0;
    int right = std::size(nums) - 1;

    while (left < right) {
        auto delta = (right - left) / 2;
        auto middle = right - delta;
        if(delta == 0) /* edge case */
        {
            if(nums[left] == element)
            {
                return left;
            }
            else if (nums[right] == element)
            {
                return right;
            }
        }

        std::cout 
            << "--> delta=" << delta
            << " middle=" << middle
            << " left= " << left 
            << " right=" << right 
            << " searching for " << element
            << std::endl;
        if (nums[middle] > element) {
            right = middle;
        } else if (nums[middle] < element) {
            left = middle;
        } else {
            return middle;
        }
    }

    return {};
}

void FindIndex(std::vector<int> elements, int element)
{
    std::cout << "searching for element: " << element << std::endl;
    auto res = BinarySearch(elements, element);
    std::cout << " result= " << (res.has_value() ? std::to_string(elements[*res]) : "nothing");
    std::cout << std::endl;
}

void DrawLine()
{
    for(int i = 0; i < 20; ++i)
        std::cout << "-";

    std::cout << std::endl;
}

int main()
{
    std::vector<int> nums = { 1, 3, 5, 7, 9, 20, 53 };
    std::cout << "[";
    for (auto n : nums) {
        std::cout << " " << n;
    }
    std::cout << " ]";
    std::cout << std::endl;

    FindIndex(nums, 7);
    FindIndex(nums, 1);
    FindIndex(nums, 53);
}
