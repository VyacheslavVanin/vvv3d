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
        {"value", setValue},
        {"action", setAction},
        {"actions", setAction},
        {"on_enter", setOnEnterAction},
        {"on_change", setOnValueChangedAction},
    }, custom_mapping(), standard_vector_mapping{
        {"size", setSize},
        {"max_size", setMaxSize},
        {"min_size", setMinSize},
        {"pos", setPosition},
        {"position", setPosition},
        {"color", setColorv},
        {"background_color", setBGColorv},
        {"bgcolor", setBGColorv},
        {"cursor_color", setCursorColorv},
        {"action", setActions},
        {"actions", setActions},
        {"on_enter", setOnEnterActions},
        {"on_change", setOnValueChangedActions},
    }, custom_vector_mapping()
{
}

void PropertyMapper::addProperty(
    const std::string& property_name,
    const std::function<void(vvv3d::Widget*, const std::string&)>& func)
{
    custom_mapping[property_name] = func;
}

void PropertyMapper::addProperty(const std::string& property_name,
                                 const func_vector_t& func)
{
    custom_vector_mapping[property_name] = func;
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

bool apply(const PropertyMapper::mapping_vector& mapping, vvv3d::Widget* w,
           const std::string& property, const std::vector<std::string>& value)
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

void PropertyMapper::applyProperty(vvv3d::Widget* w,
                                   const std::string& property_name,
                                   const std::vector<std::string>& value) const
{
    apply(standard_vector_mapping, w, property_name, value) ||
        apply(custom_vector_mapping, w, property_name, value);
}

void PropertyMapper::applyProperty(vvv3d::Widget* w,
                                   const std::string& property_name,
                                   const vvv::CfgNode::value_type& value) const
{
    if (value.isString())
        applyProperty(w, property_name, value.asString());
    else if (value.isList())
        applyProperty(w, property_name, value.asStringList());
}

PropertyMapper& PropertyMapper::instance()
{
    static PropertyMapper mapper;
    return mapper;
}

} // namespace vvv3d
