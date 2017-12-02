#include "property_mapper.hpp"

#include <iostream>

#include "standard_properties.hpp"

namespace vvv3d {

using namespace vvv3d::properties;
PropertyMapper::PropertyMapper() : standard_mapping
    {
        {"text", setText},
        {"color", setColor},
        {"width", setWidth},
        {"height", setHeight},
        {"max_width", setMaxWidth},
        {"max_height", setMaxHeight},
        {"min_width", setMinWidth},
        {"min_height", setMinHeight},
        {"x", setPosX},
        {"y", setPosY},
        {"pos_x", setPosX},
        {"pos_y", setPosY},
        {"visible", setVisible},
        {"padding", setPadding},
        {"border", setBorder},
        {"expand", setExpand},
        {"valign", setVAlign},
        {"halign", setHAlign},
        {"autoresize", setAutoResize},
        {"font", setFont},
        {"background_color", setBGColor},
        {"bgcolor", setBGColor},
        {"cursor_color", setCursorColor},
        {"range", setRange},
        {"value", setValue},
    }, custom_mapping()
{
}

void PropertyMapper::addProperty(
    const std::string& property_name,
    const std::function<void(vvv3d::Widget*, const std::string&)>& func)
{
    custom_mapping[property_name] = func;
}

namespace {
bool apply(const PropertyMapper::mapping& mapping, vvv3d::Widget* w,
           const std::string& property, const std::string& value)
{
    auto it = mapping.find(property);
    if (it == mapping.end())
        return false;
    it->second(w, value);
    return true;
}
} // namespace

void PropertyMapper::applyProperty(vvv3d::Widget* w,
                                   const std::string& property_name,
                                   const std::string& value) const
{
    apply(standard_mapping, w, property_name, value) ||
        apply(custom_mapping, w, property_name, value);
}

PropertyMapper& PropertyMapper::instance()
{
    static PropertyMapper mapper;
    return mapper;
}

}
