#include <algorithm>
#include <iostream>
#include <vector>

void print(const std::vector<int>& ns)
{
    std::cout << "[ ";
    for (auto n : ns) {
        std::cout << n << " ";
    }
    std::cout << "]" << std::endl;
}

void print(
    std::vector<int>::iterator begin,
    std::vector<int>::iterator end)
{
    std::cout << "[ ";
    while (begin != end) {
        std::cout << *begin << " ";
        begin++;
    }
    std::cout << "]" << std::endl;
}

void Sort(
    std::vector<int>::iterator b,
    std::vector<int>::iterator e)
{
    // pick a pivot
    auto delta = std::distance(b, e) / 2;
    if (delta == 0)
        return;

    auto pivot = *(b + delta);

    // put all elemets larger then the pivot, before the pivot.
    // Put all others behind the pivot
    auto left = b;
    auto right = e - 1;
    while (left != right) {
        while (left != right && (*left) < pivot) {
            left++;
        }

        while (left != right && (*right) > pivot) {
            right--;
        }

        std::iter_swap(left, right);
    }
    Sort(b, left);
    Sort(left, e);
}

int main()
{
    std::vector<int> nums = { 3, 7, 2, 8, 0, 1, 6 };
    std::cout << "expected:" <<std::endl;
    print(nums);
    std::cout << "result:" <<std::endl;
    Sort(nums.begin(), nums.end());
    print(nums);

    std::cout
        << "result is "
        << (std::is_sorted(nums.begin(), nums.end()) ? "ok" : "not ok")
        << std::endl;
}
