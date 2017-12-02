#pragma once
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/std/gui/gui_layer_base.hpp>
#include <vvvcfg/vvvcfg.hpp>

#include <functional>
#include <unordered_map>

namespace vvv3d {

class GuiLayer : public GuiLayerBase {
public:
    using widget_type = vvv3d::Widget*;

    void load(const std::string& config);
    void load(std::istream& stream);
    void clear();

    template <typename T>
    T* get(const std::string& name) const;
    widget_type getByShortName(const std::string& name) const;
    widget_type getByFullName(const std::string& name) const;

    const std::vector<widget_type>& topWidgets() const;

    void setText(const std::string& widget_name, const std::string& text);
    std::string getText(const std::string& widget_name) const;

    void setColor(const std::string& widget_name, const vvv3d::Color& color);

    static void register_property(
        const std::string& property_name,
        const std::function<void(vvv3d::Widget*, const std::string&)>&);

private:
    std::unordered_map<std::string, widget_type> short_name_map;
    std::unordered_map<std::string, widget_type> full_name_map;
    std::vector<widget_type> top_widgets;

    void makeSubWidgets(vvv3d::Widget* w, const vvv::CfgNode& node);

    widget_type addWidgetNode(const vvv::CfgNode& node);
};

template <typename T>
T* GuiLayer::get(const std::string& name) const
{
    T* ret = dynamic_cast<T*>(getByShortName(name));
    if (ret != nullptr)
        return ret;
    return dynamic_cast<T*>(getByFullName(name));
}

} // namespace vvv3d
