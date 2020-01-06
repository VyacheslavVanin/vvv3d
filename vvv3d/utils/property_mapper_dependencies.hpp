#pragma once
#include "property_mapper.hpp"

/// TODO: Move to stdhelper
namespace {
template <typename U>
inline const typename U::mapped_type& getSafe(const U& map,
                                              const typename U::key_type& key)
{
    static const typename U::mapped_type DEFAULT{};
    const auto it = map.find(key);
    if (it == map.end())
        return DEFAULT;

    return it->second;
}

template <typename U>
inline std::vector<typename U::key_type> keys(const U& map)
{
    std::vector<typename U::key_type> ret;
    for (const auto& e : map)
        ret.push_back(e.first);
    return ret;
}
} // namespace

namespace vvv3d {

struct PropertyDependencies {
public:
    using prop_type = std::string;
    using prop_list_type = std::unordered_set<prop_type>;
    using dependencies_type = std::unordered_map<prop_type, prop_list_type>;

    inline void addDependency(const prop_type& prop,
                              const prop_type& depends_of);
    inline void addDependency(const prop_type& prop,
                              const prop_list_type& dependes_of);

    /// Check property has all dependecies satisfied
    /// return true if all \p prop dependecies from \p all_props exists in \p
    /// processed props
    /// Any \p prop dependecies not listed in \p all_props ignored
    inline bool
    satisfiedDependency(const prop_type& prop,
                        const vvv::CfgNode::properties_type& all_props,
                        const prop_list_type& processed_props) const;

private:
    dependencies_type all_deps;
};

inline bool PropertyDependencies::satisfiedDependency(
    const prop_type& prop, const vvv::CfgNode::properties_type& all_props,
    const prop_list_type& processed_props) const
{
    const auto& deps = getSafe(all_deps, prop);
    if (deps.empty())
        return true;

    // has not processed dependecy
    for (const auto& dep : deps) {
        if (all_props.count(dep) && !processed_props.count(dep))
            return false;
    }

    return true;
}

inline void PropertyDependencies::addDependency(const prop_type& prop,
                                                const prop_type& depends_of)
{
    all_deps[prop].insert(depends_of);
}

inline void
PropertyDependencies::addDependency(const prop_type& prop,
                                    const prop_list_type& depends_of)
{
    all_deps[prop].insert(depends_of.begin(), depends_of.end());
}

template <typename T>
inline vvv::CfgNode::properties_type
applyProperties(T* o, const vvv::CfgNode::properties_type& props,
                const PropertyDependencies& all_deps,
                std::unordered_set<std::string>& processed)
{
    const auto& prop_mapper = vvv3d::PropertyMapper<T>::instance();
    vvv::CfgNode::properties_type delayed;

    for (const auto& prop : props) {
        const auto& name = prop.first;
        const auto& value = prop.second;

        if (all_deps.satisfiedDependency(name, props, processed)) {
            prop_mapper.applyProperty(o, name, value);
            processed.insert(name);
        }
        else {
            delayed.insert(prop);
        }
    }

    return delayed;
}

template <typename T>
inline void applyProperties(T* o, const vvv::CfgNode::properties_type& props,
                            const PropertyDependencies& all_deps)
{
    std::unordered_set<std::string> processed;

    auto properties = props;
    vvv::CfgNode::properties_type delayed_properties;
    while (properties.size()) {
        delayed_properties =
            applyProperties(o, properties, all_deps, processed);
        if (properties.size() == delayed_properties.size()) {
            // TODO: make more informative message
            LOG("failed apply properties: cyclic dependency");
            return;
        }
        properties = std::move(delayed_properties);
    }
}
} // namespace vvv3d
