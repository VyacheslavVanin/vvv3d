#include "gui_layer.hpp"

#include <unordered_map>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvv3dgui.hpp>
#include <vvvcfg/vvvcfg.hpp>

#include "property_mapper.hpp"
#include "widget_fabric.hpp"

namespace vvv3d {
namespace {
void log(const std::string& msg) { std::cerr << msg << "\n"; }
} // namespace

vvv3d::Widget* makeWidget(const vvv::CfgNode& node)
{
    using namespace std;
    using namespace vvv3d;
    using vvv::CfgNode;
    const auto& properties = node.getProperties();
    const auto& type_it = properties.find("type");
    if (type_it == properties.end()) {
        log("Type of \"" + node.getName() + "\" not specified");
        return nullptr;
    }
    const auto& type = type_it->second;
    auto ret = vvv3d::WidgetFabric::instance().create(type);
    for (const auto& prop : node.getProperties()) {
        const auto& name = prop.first;
        const auto& value = prop.second;
        PropertyMapper::instance().applyProperty(ret, name, value);
    }
    return ret;
}

////
//
//  GuiLayer
//
////

void GuiLayer::register_property(
    const std::string& property_name,
    const std::function<void(vvv3d::Widget*, const std::string&)>& func)
{
    PropertyMapper::instance().addProperty(property_name, func);
}

void GuiLayer::load(const std::string& config)
{
    std::stringstream ss;
    ss << config;
    load(ss);
}

void GuiLayer::makeSubWidgets(vvv3d::Widget* w, const vvv::CfgNode& node)
{
    auto container = dynamic_cast<vvv3d::IContainer*>(w);
    if (!container)
        return;
    for (const auto& subnode : node.getChildren())
        container->addWidget(addWidgetNode(subnode));
}

vvv3d::Widget* GuiLayer::addWidgetNode(const vvv::CfgNode& node)
{
    auto w = makeWidget(node);
    const auto& name = node.getName();
    short_name_map[name] = w;
    makeSubWidgets(w, node);
    return w;
}

void GuiLayer::load(std::istream& stream)
{
    const auto& cfg = vvv::make_cfg(stream);
    for (const auto& c : cfg.getChild("forms").getChildren()) {
        auto w = addWidgetNode(c);
        top_widgets.push_back(w);
    }
    for (auto w: topWidgets())
        addWidget(w);
}

void GuiLayer::clear()
{
    for (auto w: topWidgets()) {
        removeWidget(w);
        delete w;
    }
    top_widgets.clear();
}

GuiLayer::widget_type
GuiLayer::getByShortName(const std::string& name) const
{
    auto it = short_name_map.find(name);
    if (it != short_name_map.end())
        return it->second;
    return nullptr;
}

GuiLayer::widget_type
GuiLayer::getByFullName(const std::string& name) const
{
    auto it = full_name_map.find(name);
    if (it != full_name_map.end())
        return it->second;
    return nullptr;
}

const std::vector<GuiLayer::widget_type>& GuiLayer::topWidgets() const
{
    return top_widgets;
}

void GuiLayer::setText(const std::string& widget_name,
                          const std::string& text)
{
    auto w = get<vvv3d::ITextProperty>(widget_name);
    if (w)
        return w->setText(text);
    log("Error: \"" + widget_name + "\" does not implement ITextProperty");
}

std::string GuiLayer::getText(const std::string& widget_name) const
{
    auto w = get<vvv3d::ITextProperty>(widget_name);
    if (w)
        return w->getText();
    log("Error: \"" + widget_name + "\" does not implement ITextProperty");
    return {};
}

void GuiLayer::setColor(const std::string& widget_name,
                           const vvv3d::Color& color)
{
    auto w = get<vvv3d::IColorProperty>(widget_name);
    if (w)
        return w->setColor(color);
    log("Error: \"" + widget_name + "\" does not implement IColorProperty");
}
}
