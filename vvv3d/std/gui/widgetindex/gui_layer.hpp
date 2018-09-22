#pragma once
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/std/gui/gui_layer_base.hpp>
#include <vvvcfg/vvvcfg.hpp>

#include <functional>
#include <unordered_map>

namespace vvv3d {

class GuiLayer : public GuiLayerBase {
public:
    using widget_type = vvv3d::Widget;
    using widget_ptr_type = widget_type*;

    void load(const std::string& config);
    void load(std::istream& stream);
    void load(const vvv::CfgNode& config);
    void clear();

    template <typename T = widget_type>
    T* get(const std::string& name) const;
    widget_ptr_type getByShortName(const std::string& name) const;
    widget_ptr_type getByFullName(const std::string& name) const;

    const std::vector<widget_ptr_type>& topWidgets() const;

    std::string getText(const std::string& widget_name) const;
    void setText(const std::string& widget_name, const std::string& text);
    inline void setText(const std::string& widget_name, const char* text) {
        setText(widget_name, std::string(text));
    }
    template<typename T>
    inline void setText(const std::string& widget_name, const T& value) {
        setText(widget_name, std::to_string(value));
    }

    void setColor(const std::string& widget_name, const vvv3d::Color& color);

    static void register_property(
        const std::string& property_name,
        const std::function<void(vvv3d::Widget*, const std::string&)>&);

private:
    std::unordered_map<std::string, widget_ptr_type> short_name_map;
    std::unordered_map<std::string, widget_ptr_type> full_name_map;
    std::vector<widget_ptr_type> top_widgets;

    void makeSubWidgets(vvv3d::Widget* w, const vvv::CfgNode& node);

    widget_ptr_type addWidgetNode(const vvv::CfgNode& node);
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
