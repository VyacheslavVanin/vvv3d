#include "standard_properties.hpp"
#include <iostream>

namespace vvv3d {

namespace {
void log(const std::string& text) { std::cerr << text << "\n"; }

auto DEFAULT_COLOR = vvv3d::WHITE;
}; // namespace

bool str_to_bool(const std::string& value)
{
    using list = std::initializer_list<std::string>;
    static const list t = {"", "true", "True", "yes", "Yes"};
    static const list f = {"false", "False", "no", "No"};
    if (std::find(t.begin(), t.end(), value) != t.end())
        return true;
    if (std::find(f.begin(), f.end(), value) != f.end())
        return false;
    log("String \"" + value + "\" is not convertable to bool");
    return false;
}

vvv3d::HALIGN str_to_halign(const std::string& value)
{
    if (value == "fill")
        return vvv3d::HALIGN::FILL;
    if (value == "center")
        return vvv3d::HALIGN::CENTER;
    if (value == "left")
        return vvv3d::HALIGN::LEFT;
    if (value == "right")
        return vvv3d::HALIGN::RIGHT;
    log("String \"" + value + "\" can't be converted to HALIGN");
    return vvv3d::HALIGN::CENTER;
}

vvv3d::VALIGN str_to_valign(const std::string& value)
{
    if (value == "fill")
        return vvv3d::VALIGN::FILL;
    if (value == "center")
        return vvv3d::VALIGN::CENTER;
    if (value == "top")
        return vvv3d::VALIGN::TOP;
    if (value == "bottom")
        return vvv3d::VALIGN::BOTTOM;
    log("String \"" + value + "\" can't be converted to VALIGN");
    return vvv3d::VALIGN::CENTER;
}

namespace properties {
void setText(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::ITextProperty*>(property);
    if (w)
        w->setText(value);
}

void setColor(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IColorProperty*>(property);
    if (w)
        w->setColor(to_color(value));
}

void setBGColor(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IBGColorProperty*>(property);
    if (w)
        w->setBGColor(to_color(value));
}

void setCursorColor(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::ICursorColorProperty*>(property);
    if (w)
        w->setCursorColor(to_color(value));
}

void setWidth(vvv3d::Widget* property, const std::string& value)
{
    property->setWidth(std::stoi(value));
}

void setHeight(vvv3d::Widget* property, const std::string& value)
{
    property->setHeight(std::stoi(value));
}

void setMaxWidth(vvv3d::Widget* property, const std::string& value)
{
    property->setMaxWidth(std::stoi(value));
}

void setMaxHeight(vvv3d::Widget* property, const std::string& value)
{
    property->setMaxHeight(std::stoi(value));
}

void setMinWidth(vvv3d::Widget* property, const std::string& value)
{
    property->setMinWidth(std::stoi(value));
}

void setMinHeight(vvv3d::Widget* property, const std::string& value)
{
    property->setMinHeight(std::stoi(value));
}

void setVisible(vvv3d::Widget* property, const std::string& value)
{
    property->setVisible(str_to_bool(value));
}

void setPosX(vvv3d::Widget* property, const std::string& value)
{
    property->setPosition(std::stoi(value), property->getPosition().y);
}

void setPosY(vvv3d::Widget* property, const std::string& value)
{
    property->setPosition(property->getPosition().x, std::stoi(value));
}

void setPadding(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IPaddingProperty*>(property);
    if (w)
        w->setPadding(std::stoi(value));
}

void setBorder(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IBorderProperty*>(property);
    if (w)
        w->setBorder(std::stoi(value));
}

void setExpand(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IExpandProperty*>(property);
    if (w)
        w->setExpandToContent(str_to_bool(value));
}

void setVAlign(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IVAlignProperty*>(property);
    if (w)
        w->setVAlign(str_to_valign(value));
}

void setHAlign(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IHAlignProperty*>(property);
    if (w)
        w->setHAlign(str_to_halign(value));
}

void setAutoResize(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IAutoResizeProperty*>(property);
    if (w)
        w->setAutoResize(str_to_bool(value));
}

void setFont(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IFontProperty*>(property);
    if (!w)
        return;
    auto& rm = vvv3d::Engine::getActiveEngine().getResourceManager();
    const auto& font = rm.getFontManager().getFont(value);
    w->setFont(font);
}

void setRange(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IRangeProperty*>(property);
    if (w)
        w->setRange(std::stoi(value));
}

void setValue(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IValueProperty*>(property);
    if (w)
        w->setValue(std::stoi(value));
}
} // namespace properties
} // namespace vvv3d
