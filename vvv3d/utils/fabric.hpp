#pragma once
#include <functional>
#include <unordered_map>

namespace vvv3d {
template <typename T> class Fabric {
public:
    using func_t = std::function<T()>;
    using mapping = std::unordered_map<std::string, func_t>;

    static Fabric& instance()
    {
        static Fabric<T> fabric;
        return fabric;
    }

    void addType(const std::string& name, const func_t& fabric)
    {
        custom_map[name] = fabric;
    }

    T create(const std::string& name) const
    {
        auto ret = create(standard_map, name);
        if (!ret)
            ret = create(custom_map, name);

        return ret;
    }

private:
    Fabric();
    Fabric(const Fabric&) = delete;
    Fabric(Fabric&&) = delete;
    Fabric& operator=(const Fabric&) = delete;
    Fabric& operator=(Fabric&&) = delete;

    static T create(const mapping& map, const std::string& name)
    {
        auto it = map.find(name);
        if (it == map.end())
            return nullptr;
        return it->second();
    }

    mapping standard_map;
    mapping custom_map;
};
} // namespace vvv3d
