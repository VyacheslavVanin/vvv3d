#include "iaction.hpp"
#include <vvv3d/std/gui/actionmap.hpp>

namespace vvv3d {
template <typename T>
class Actions {
public:
    template <typename... Args>
    static void invoke(const std::string& actionName, Args&&... args)
    {
        get().invoke(actionName, args...);
    }

    template <typename... Args>
    static void invoke(const std::vector<std::string>& action_names,
                       Args&&... args)
    {
        for (const auto& name : action_names)
            invoke(name, args...);
    }

    static void add(const std::string& name, const std::function<T>& f)
    {
        get().addAction(name, f);
    }

    static ActionMap<T>& debug() { return get(); }

private:
    static ActionMap<T>& get()
    {
        static ActionMap<T> instance;
        return instance;
    }
};

void addAction(const std::string& name, const std::function<void(void)>& f)
{
    vvv3d::Actions<void(void)>::add(name, f);
}

void addAction(const std::string& name,
               const std::function<void(const std::string&)>& f)
{
    vvv3d::Actions<void(const std::string&)>::add(name, f);
}

void addAction(const std::string& name, const std::function<void(int)>& f)
{
    vvv3d::Actions<void(int)>::add(name, f);
}

void invokeAction(const std::string& name)
{
    Actions<void(void)>::invoke(name);
}
void invokeAction(const std::string& name, int value)
{
    Actions<void(int)>::invoke(name, value);
}
void invokeAction(const std::string& name, const std::string& value)
{
    Actions<void(const std::string&)>::invoke(name, value);
}
void invokeAction(const std::vector<std::string>& name)
{
    Actions<void(void)>::invoke(name);
}
void invokeAction(const std::vector<std::string>& name, int value)
{
    Actions<void(int)>::invoke(name, value);
}
void invokeAction(const std::vector<std::string>& name,
                  const std::string& value)
{
    Actions<void(const std::string&)>::invoke(name, value);
}

} // namespace vvv3d
