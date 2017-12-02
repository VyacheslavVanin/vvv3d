#include "standard_properties.hpp"
#include <boost/endian/conversion.hpp>

namespace vvv3d {

bool str_to_bool(const std::string& value)
{
    using list = std::initializer_list<std::string>;
    static const list t = {"", "true", "True", "yes", "Yes"};
    static const list f = {"false", "False", "no", "No"};
    if (std::find(t.begin(), t.end(), value) != t.end())
        return true;
    if (std::find(f.begin(), f.end(), value) != f.end())
        return false;
    throw std::invalid_argument("String \"" + value +
                                "\" is not convertable to bool");
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
    throw std::invalid_argument("String \"" + value +
                                "\" can't be converted to HALIGN");
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
    throw std::invalid_argument("String \"" + value +
                                "\" can't be converted to VALIGN");
}

vvv3d::Color str_to_color(const std::string& value)
{
    static const float unit16 = 1.0f / 0xFF;
    const auto prefix_0x = (value.find("0x") == 0);
    const auto& len = value.size() - prefix_0x * 2;
    if (len != 6 && len != 8)
        throw std::invalid_argument("String \"" + value + "\" of len " +
                                    std::to_string(len) +
                                    "can't be cast to Color");
    size_t pos = 0;
    const auto int_value = std::stoi(value, &pos, 16);
    const auto with_alpha = (len == 8) ? int_value : int_value << 8 | 0xff;

    using boost::endian::native_to_big;
    const auto color = native_to_big(with_alpha);
    if (pos != value.size())
        throw std::invalid_argument(
            "String \"" + value +
            "\" contain invalid symbols, so it can't be cast to Color");

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&color);
    return vvv3d::Color(bytes[0] * unit16, bytes[1] * unit16, bytes[2] * unit16,
                        bytes[3] * unit16);
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
        w->setColor(str_to_color(value));
}

void setBGColor(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::IBGColorProperty*>(property);
    if (w)
        w->setBGColor(str_to_color(value));
}

void setCursorColor(vvv3d::Widget* property, const std::string& value)
{
    auto w = dynamic_cast<vvv3d::ICursorColorProperty*>(property);
    if (w)
        w->setCursorColor(str_to_color(value));
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
