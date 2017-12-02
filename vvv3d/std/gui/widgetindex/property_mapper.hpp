#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#include <vvv3d/std/gui/widget.hpp>

namespace vvv3d {
class PropertyMapper {
public:
    using func_t = std::function<void(vvv3d::Widget*, const std::string&)>;
    using mapping = std::unordered_map<std::string, func_t>;

    static PropertyMapper& instance();
    void addProperty(const std::string& property_name, const func_t& func);

    void applyProperty(vvv3d::Widget* w, const std::string& property_name,
                       const std::string& value) const;

private:
    PropertyMapper();
    PropertyMapper(const PropertyMapper&) = delete;
    PropertyMapper(PropertyMapper&&) = delete;
    PropertyMapper& operator=(const PropertyMapper&) = delete;
    PropertyMapper& operator=(PropertyMapper&&) = delete;

    mapping standard_mapping;
    mapping custom_mapping;
};
} // namespace vvv3d
