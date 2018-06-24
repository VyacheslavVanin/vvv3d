#pragma once
#include <functional>
#include <iostream>
#include <vector>

namespace vvv3d {

/**
Usage example:
int main()
{
    using namespace std;
    actionList<void(void)> vv;
    vv.addAction([](){std::cout << "action1\n";});
    vv.addAction([](){std::cout << "action2\n";});
    vv.addAction([](){std::cout << "action3\n";});
    vv();

    actionList<void(const char*)> vcc;
    vcc.addAction([](const char* s) { cout << "params passed: " << s << "\n";});
    vcc.addAction([](const char* s) { cout << "Hello, " << s << "!!!\n";});
    vcc("World");
    vcc("Man");
}
*/

template <typename T>
class ActionList {
public:
    void addAction(const std::function<T>& f) { actions.push_back(f); }

    template <typename... Args>
    void operator()(const Args&... args) const
    {
        for (const auto& a : actions)
            a(args...);
    }

private:
    std::vector<std::function<T>> actions = {};
};

} // namespace vvv3d
