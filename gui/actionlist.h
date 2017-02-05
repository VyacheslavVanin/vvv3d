#ifndef ACTIONLIST_H
#define ACTIONLIST_H
#include <iostream>
#include <functional>
#include <vector>

/**
Usage example:
int main()
{
    actionList<void(void)> vv;
    vv.addAction([](){std::cout << "action1\n";});
    vv.addAction([](){std::cout << "action2\n";});
    vv.addAction([](){std::cout << "action3\n";});
    vv.invoke();

    actionList<void(const char*)> vcc;
    vcc.addAction([](const char* s) {std::cout << "parameter passed: " << s << "\n";});
    vcc.addAction([](const char* s) {std::cout << "Hello, " << s << "!!!\n";});
    vcc.invoke("World");
    vcc.invoke("Man");
}
*/

template<typename T>
class ActionList
{
    public:
        void addAction(const std::function<T>& f)
        {
            actions.push_back(f);
        }

        template<typename... Args>
        void invoke(Args&... args)
        {
            for(const auto& a: actions)
                a(args...);
        }

    private:
        std::vector<std::function<T>> actions = {};
};

#endif // ACTIONLIST_H
