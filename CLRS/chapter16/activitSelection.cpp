#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

struct Activity {
    int start;
    int stop;
};

bool isOverlapping(
    const Activity& left,
    const Activity& right)
{
    return left.start >= right.stop
        || left.stop >= right.start;
    // in all other cases we have overlapping...
}

std::string toString(const Activity& act)
{
    std::stringstream ss;
    ss << "[ start=" << act.start << "; stop=" << act.stop << "]";

    return ss.str();
}

void print(std::vector<Activity>& acts)
{
    std::cout << std::endl;
    for (auto a : acts) {
        std::cout << toString(a) << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    // example page 415:
    std::vector<Activity> activities = {
        { 1, 4 },
        { 3, 5 },
        { 0, 6 },
        { 5, 7 },
        { 3, 9 },
        { 5, 9 },
        { 6, 10 },
        { 8, 11 },
        { 8, 12 },
        { 2, 14 },
        { 12, 16 }
    };
    std::sort(activities.begin(), activities.end(), [](auto l, auto r) { return l.stop < r.stop; });
    print(activities);

    std::vector<Activity> schedule; // start with empty schedule
    auto cursor = activities.begin();
    while (cursor != activities.end()) {
        schedule.push_back(*cursor);
        cursor = std::lower_bound(cursor+1, activities.end(), cursor->stop,
            [](const Activity& act, int val) { return act.start < val; });
    }

    print(schedule);
    return 0;
}
