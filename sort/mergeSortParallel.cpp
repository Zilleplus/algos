#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>

std::vector<int> CreateVector(const std::size_t size)
{
    std::vector<int> numbers;
    numbers.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        numbers.push_back(i);
    }

    return numbers;
}

template <typename T>
std::string ToString(const std::vector<T>& vec)
{
    std::stringstream ss;
    ss << "[";
    if (!vec.empty()) {
        ss << vec.front() << " ";

        auto b = vec.begin() + 1;
        auto e = vec.end();
        while (b != e) {
            ss << ", " << *b;
            b++;
        }
    }

    ss << "]";

    return ss.str();
}

template <typename TIterator>
auto Merge(
    TIterator left_begin,
    TIterator left_end,
    TIterator right_begin,
    TIterator right_end)
{
    std::vector<int> merged;

    while (left_begin != left_end || right_begin != right_end) {
        auto left_ready = left_begin != left_end;
        auto right_ready = right_begin != right_end;

        if (left_ready && right_ready) {
            if (*left_begin >= *right_begin) {
                merged.push_back(*right_begin);
                ++right_begin;
            } else if (*left_begin < *right_begin) {
                merged.push_back(*left_begin);
                ++left_begin;
            }
        } else if (left_ready) {
            std::copy(left_begin, left_end, std::back_inserter(merged));
            std::cout
                << "merge ends in left with size="
                << std::size(merged)
                << " with result ="
                << ToString(merged)
                << "\n\r";
            return merged;
        } else if (right_ready) {
            std::copy(right_begin, right_end, std::back_inserter(merged));
            std::cout
                << "merge ends in right with size="
                << std::size(merged)
                << " with result ="
                << ToString(merged)
                << "\n\r";
            return merged;
        }
    }

    return merged;
}

template <typename TIterator>
auto MergeSort(
    std::size_t size,
    TIterator begin,
    TIterator end)
{
    if (size == 0) {
        std::vector<int> v;
        return v;
    }
    if (size == 1) {
        std::vector<int> v;
        v.push_back(*begin);
        return v;
    }

    auto left_size = size / std::size_t { 2 };
    auto right_size = size - left_size;
    auto middle = begin + left_size;

    auto&& left = std::async(
        std::launch::async,
        [&]() { return MergeSort(left_size, begin, middle); });
    auto&& right = std::async(
        std::launch::async,
        [&]() { return MergeSort(right_size, middle, end); });

    // We can't get the result twice, so assign it here first.
    // Then later get begin/end iterator.
    auto left_result = left.get();
    auto right_result = right.get();

    auto&& merged_vec = Merge(
        left_result.cbegin(),
        left_result.cend(),
        right_result.cbegin(),
        right_result.cend());

    return merged_vec;
}

int main()
{
    const std::size_t size = 10;

    auto sorted_numbers = CreateVector(size);
    std::cout << "vec:" << ToString(sorted_numbers) << "\n\r";

    std::random_device rd;
    std::mt19937 g(rd());
    auto random_numbers = CreateVector(size);
    std::shuffle(random_numbers.begin(), random_numbers.end(), g);
    std::cout << "shuffled vec:" << ToString(random_numbers) << "\n\r";

    std::cout << "sorting random numbers..."
              << "\n\n\n\r" << std::flush;
    auto result = MergeSort(
        std::size(random_numbers),
        random_numbers.cbegin(),
        random_numbers.cend());

    std::cout << "sorted:" << ToString(result) << "\n\r";

    auto eq = std::equal(sorted_numbers.cbegin(), sorted_numbers.cend(), result.cbegin());
    std::cout << "sort = " << (eq ? "OK" : "NOT OK") << "\n\r";

    std::cout << std::flush;

    return 0;
}
