#pragma once
#include "actionlist.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace vvv3d {

template <typename T> class ActionMap {
public:
    void addAction(const std::string& name, const std::function<T>& f)
    {
        actions[name].addAction(f);
    }

    template <typename... Args>
    void invoke(const std::string& name, Args&... args) const
    {
        const auto& it = actions.find(name);
        if (it == actions.end())
            return;
        it->second(args...);
    }

    const ActionList<T>& operator[](const std::string& name) const
    {
        return actions.at(name);
    }

private:
    std::unordered_map<std::string, ActionList<T>> actions = {};
};

} // namespace vvv3d
