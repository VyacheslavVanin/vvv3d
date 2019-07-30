#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/std/log.hpp>
#include <vvvstdhelper/containerhelper.hpp>

namespace vvv3d {

template <typename T> class BaseManager {
public:
    using uniq_res = std::unique_ptr<T>;
    using shared_res = std::shared_ptr<T>;
    using init_func_t = std::function<std::unique_ptr<T>(void)>;

    inline void add(const std::string& name, shared_res res);
    inline void add(const std::string& name, uniq_res res);
    inline void add(const std::string& name, const init_func_t& f);
    inline const T& get(const std::string& name) const;
    inline T& get(const std::string& name);
    inline std::vector<std::string> listNames() const;
    inline void clear();
    inline bool contain(const std::string& name) const;

private:
    using resource_map = std::unordered_map<std::string, shared_res>;
    using initializer_map = std::unordered_map<std::string, init_func_t>;
    resource_map resources;
    initializer_map initializers;
};

template <typename T> inline void BaseManager<T>::clear() { resources.clear(); }

template <typename T>
inline std::vector<std::string> BaseManager<T>::listNames() const
{
    using vvv::helpers::map;
    using vvv::helpers::getFirst;
    return map<std::vector<std::string>>(getFirst, resources);
}

template <typename T>
inline bool BaseManager<T>::contain(const std::string& name) const
{
    return resources.find(name) != resources.end();
}

template <typename T>
void BaseManager<T>::add(const std::string& name,
                         BaseManager<T>::shared_res res)
{
    if (contain(name))
        return;

    resources[name] = std::move(res);
}

template <typename T>
void BaseManager<T>::add(const std::string& name, BaseManager<T>::uniq_res res)
{
    if (contain(name))
        return;

    resources[name].reset(res.release());
}

template <typename T>
void BaseManager<T>::add(const std::string& name,
                         const BaseManager<T>::init_func_t& res)
{
    initializers[name] = res;
}

template <typename T>
inline const T& BaseManager<T>::get(const std::string& name) const
try {
    return resources.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(vvv::helper::format("failed to get resource \"@\"", name));
}

template <typename T> inline T& BaseManager<T>::get(const std::string& name)
try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end())
            add(name, it->second());
    }
    return *resources.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(vvv::helper::format("failed to get resource \"@\"", name));
    throw;
}

} // namespace vvv3d
