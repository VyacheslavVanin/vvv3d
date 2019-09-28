#include "widget_property_mapper.hpp"

#include "standard_properties.hpp"

namespace vvv3d {

using namespace vvv3d::properties;
template<>
PropertyMapper<vvv3d::Widget>::PropertyMapper() : standard_mapping
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
        {"image", setImage},
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
} // namespace vvv3d
