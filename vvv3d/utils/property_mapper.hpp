#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#include <vvv3d/gui/widgets/widget.hpp>
#include <vvvcfg/cfg_node.hpp>

namespace vvv3d {
template <typename T> class PropertyMapper {
public:
    using func_t = std::function<void(T*, const std::string&)>;
    using func_vector_t =
        std::function<void(T*, const std::vector<std::string>&)>;
    using mapping = std::unordered_map<std::string, func_t>;
    using mapping_vector = std::unordered_map<std::string, func_vector_t>;

    static PropertyMapper<T>& instance()
    {
        static PropertyMapper<T> mapper;
        return mapper;
    }

    void addProperty(const std::string& property_name, const func_t& func)
    {
        custom_mapping[property_name] = func;
    }

    void addProperty(const std::string& property_name,
                     const func_vector_t& func)
    {
        custom_vector_mapping[property_name] = func;
    }

    void applyProperty(T* w, const std::string& property_name,
                       const std::string& value) const
    {
        apply(standard_mapping, w, property_name, value) ||
            apply(custom_mapping, w, property_name, value);
    }

    void applyProperty(T* w, const std::string& property_name,
                       const std::vector<std::string>& value) const
    {
        apply(standard_vector_mapping, w, property_name, value) ||
            apply(custom_vector_mapping, w, property_name, value);
    }

    void applyProperty(T* w, const std::string& property_name,
                       const vvv::CfgNode::value_type& value) const
    {
        if (value.isString())
            applyProperty(w, property_name, value.asString());
        else if (value.isList())
            applyProperty(w, property_name, value.asStringList());
    }

private:
    PropertyMapper();
    PropertyMapper(const PropertyMapper&) = delete;
    PropertyMapper(PropertyMapper&&) = delete;
    PropertyMapper& operator=(const PropertyMapper&) = delete;
    PropertyMapper& operator=(PropertyMapper&&) = delete;

    static bool apply(const mapping& mapping, T* w, const std::string& property,
                      const std::string& value)
    {
        auto it = mapping.find(property);
        if (it == mapping.end())
            return false;
        it->second(w, value);
        return true;
    }

    static bool apply(const mapping_vector& mapping, T* w,
                      const std::string& property,
                      const std::vector<std::string>& value)
    {
        auto it = mapping.find(property);
        if (it == mapping.end())
            return false;
        it->second(w, value);
        return true;
    }

    mapping standard_mapping;
    mapping custom_mapping;
    mapping_vector standard_vector_mapping;
    mapping_vector custom_vector_mapping;
};

} // namespace vvv3d
