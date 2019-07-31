#pragma once
#include <vvv3d/core/graphics/managers/base_manager.hpp>

namespace vvv3d {

/** @brief Same as BaseManager except that if there no key requested then
 * returned item by default key */
template <typename T> class BaseManagerSafe : public BaseManager<T> {
public:
    using uniq_res = typename BaseManager<T>::uniq_res;
    using shared_res = typename BaseManager<T>::shared_res;
    using init_func_t = typename BaseManager<T>::init_func_t;
    using create_by_name_func_t =
        std::function<shared_res(const std::string& name)>;

    /// Override in derived class if you need other default
    inline const std::string& getDefaultKey() const;
    inline const shared_res& getDefaultShared() const;
    inline const T& getDefault() const;

    // Overrided methods of BaseManager
    inline const T& get(const std::string& name) const;
    inline T& get(const std::string& name);

    inline const shared_res& getShared(const std::string& name) const;
    inline const shared_res& getShared(const std::string& name);

protected:
    inline shared_res tryCreate(const std::string& name);
    inline void addCreateFunction(const create_by_name_func_t& f);

private:
    std::vector<create_by_name_func_t> create_functions;
};

template <typename T>
inline const std::string& BaseManagerSafe<T>::getDefaultKey() const
{
    static const std::string default_key = "default";
    return default_key;
}

template <typename T>
inline const typename BaseManagerSafe<T>::shared_res&
BaseManagerSafe<T>::getDefaultShared() const
{
    return BaseManager<T>::getShared(getDefaultKey());
}

template <typename T> inline const T& BaseManagerSafe<T>::getDefault() const
{
    return BaseManager<T>::get(getDefaultKey());
}

template <typename T>
inline const typename BaseManagerSafe<T>::shared_res&
BaseManagerSafe<T>::getShared(const std::string& name) const
{
    const auto& resources = BaseManager<T>::resources;
    const auto& it = resources.find(name);
    if (it == resources.end())
        return getDefaultShared();

    return it.second;
}

template <typename T>
inline const typename BaseManagerSafe<T>::shared_res&
BaseManagerSafe<T>::getShared(const std::string& name)
{
    const auto& resources = BaseManager<T>::resources;
    const auto& it = resources.find(name);
    if (it != resources.end())
        return it->second;

    auto ret = tryCreate(name);
    return this->addForce(name, std::move(ret));
}

template <typename T>
inline const T& BaseManagerSafe<T>::get(const std::string& name) const
{
    return *getShared(name);
}

template <typename T> inline T& BaseManagerSafe<T>::get(const std::string& name)
{
    return *getShared(name);
}

template <typename T>
inline void
BaseManagerSafe<T>::addCreateFunction(const create_by_name_func_t& f)
{
    create_functions.push_back(f);
}

template <typename T>
inline typename BaseManagerSafe<T>::shared_res
BaseManagerSafe<T>::tryCreate(const std::string& name)
{
    for (auto& f : create_functions)
        try {
            return f(name);
        }
        catch (const std::exception& e) {
            LOG_ERROR(vvv::helper::format("failed to create resource \"@\": @",
                                          name, e.what()));
        }

    LOG_ERROR(
        vvv::helper::format("Use default resource instead of \"@\"", name));
    return getDefaultShared();
}

} // namespace vvv3d
