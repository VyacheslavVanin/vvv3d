#pragma once
#include <string>
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

private:
    static ActionMap<T>& get()
    {
        static ActionMap<T> instance;
        return instance;
    }
};

class IActionProperty {
public:
    virtual void setAction(const std::string& name) = 0;
    virtual ~IActionProperty() = default;
};

class IOnEnterProperty {
public:
    virtual void setOnEnterAction(const std::string& name) = 0;
    virtual ~IOnEnterProperty() = default;
};

class IOnValueChangeProperty {
public:
    virtual void setOnValueChangedAction(const std::string& name) = 0;
    virtual ~IOnValueChangeProperty() = default;
};

} // namespace vvv3d
