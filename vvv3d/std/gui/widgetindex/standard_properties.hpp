#pragma once
#include <string>

#include <vvv3d/vvv3dgui.hpp>

namespace vvv3d {

bool str_to_bool(const std::string& value);
vvv3d::HALIGN str_to_halign(const std::string& value);
vvv3d::VALIGN str_to_valign(const std::string& value);
vvv3d::Color str_to_color(const std::string& value);

namespace properties {
void setText(vvv3d::Widget* property, const std::string& value);
void setColor(vvv3d::Widget* property, const std::string& value);
void setBGColor(vvv3d::Widget* property, const std::string& value);
void setCursorColor(vvv3d::Widget* property, const std::string& value);
void setWidth(vvv3d::Widget* property, const std::string& value);
void setHeight(vvv3d::Widget* property, const std::string& value);
void setMaxWidth(vvv3d::Widget* property, const std::string& value);
void setMaxHeight(vvv3d::Widget* property, const std::string& value);
void setMinWidth(vvv3d::Widget* property, const std::string& value);
void setMinHeight(vvv3d::Widget* property, const std::string& value);
void setVisible(vvv3d::Widget* property, const std::string& value);
void setPosX(vvv3d::Widget* property, const std::string& value);
void setPosY(vvv3d::Widget* property, const std::string& value);
void setPadding(vvv3d::Widget* property, const std::string& value);
void setBorder(vvv3d::Widget* property, const std::string& value);
void setExpand(vvv3d::Widget* property, const std::string& value);
void setVAlign(vvv3d::Widget* property, const std::string& value);
void setHAlign(vvv3d::Widget* property, const std::string& value);
void setAutoResize(vvv3d::Widget* property, const std::string& value);
void setFont(vvv3d::Widget* property, const std::string& value);
void setRange(vvv3d::Widget* property, const std::string& value);
void setValue(vvv3d::Widget* property, const std::string& value);
} // namespace properties
} // namespace vvv3d
