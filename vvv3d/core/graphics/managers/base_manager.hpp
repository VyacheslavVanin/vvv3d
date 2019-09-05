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

    /// Add res to manager. If res with \p name already exists in manager then
    /// do nothing
    inline void add(const std::string& name, shared_res res);
    inline void add(const std::string& name, uniq_res res);

    /// Add initializer functor to manager. Resource will be initialized with
    /// returned value of functor \p f on first request by name \p name
    inline void add(const std::string& name, const init_func_t& f);

    /// Add res to manager. If res with \p name already exists in manager then
    /// override with new value
    inline shared_res& addForce(const std::string& name, shared_res res);
    inline shared_res& addForce(const std::string& name, uniq_res res);

    inline const T& get(const std::string& name) const;
    inline T& get(const std::string& name);

    inline const shared_res& getShared(const std::string& name) const;
    inline shared_res getShared(const std::string& name);

    inline std::vector<std::string> listNames() const;
    inline void remove(const std::string& name);
    inline void clear();
    inline bool contain(const std::string& name) const;

protected:
    using resource_map = std::unordered_map<std::string, shared_res>;
    using initializer_map = std::unordered_map<std::string, init_func_t>;
    resource_map resources;
    initializer_map initializers;
};

template <typename T>
inline void BaseManager<T>::remove(const std::string& name)
{
    resources.erase(name);
}

template <typename T> inline void BaseManager<T>::clear() { resources.clear(); }

template <typename T>
inline std::vector<std::string> BaseManager<T>::listNames() const
{
    using vvv::helpers::getFirst;
    using vvv::helpers::map;
    return map<std::vector<std::string>>(getFirst, resources);
}

template <typename T>
inline bool BaseManager<T>::contain(const std::string& name) const
{
    return resources.find(name) != resources.end();
}

template <typename T>
inline typename BaseManager<T>::shared_res&
BaseManager<T>::addForce(const std::string& name,
                         BaseManager<T>::shared_res res)
{
    auto& ret = resources[name];
    ret = std::move(res);
    return ret;
}

template <typename T>
inline void BaseManager<T>::add(const std::string& name,
                                BaseManager<T>::shared_res res)
{
    if (contain(name))
        return;

    addForce(name, std::move(res));
}

template <typename T>
inline typename BaseManager<T>::shared_res&
BaseManager<T>::addForce(const std::string& name, BaseManager<T>::uniq_res res)
{
    auto& ret = resources[name];
    ret.reset(res.release());
    return ret;
}

template <typename T>
inline void BaseManager<T>::add(const std::string& name,
                                BaseManager<T>::uniq_res res)
{
    if (contain(name))
        return;

    addForce(name, std::move(res));
}

template <typename T>
inline void BaseManager<T>::add(const std::string& name,
                                const BaseManager<T>::init_func_t& res)
{
    initializers[name] = res;
}

template <typename T>
inline const typename BaseManager<T>::shared_res&
BaseManager<T>::getShared(const std::string& name) const
try {
    return resources.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(
        vvv::helper::format("failed to get resource \"@\": @", name, e.what()));
    throw;
}

template <typename T>
inline typename BaseManager<T>::shared_res
BaseManager<T>::getShared(const std::string& name)
try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end())
            add(name, it->second());
    }
    return resources.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(
        vvv::helper::format("failed to get resource \"@\": @", name, e.what()));
    throw;
}

template <typename T>
inline const T& BaseManager<T>::get(const std::string& name) const
{
    return *getShared(name);
}

template <typename T> inline T& BaseManager<T>::get(const std::string& name)
{
    return *getShared(name);
}

} // namespace vvv3d
