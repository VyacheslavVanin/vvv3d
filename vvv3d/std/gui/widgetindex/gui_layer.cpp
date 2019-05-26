#include "gui_layer.hpp"

#include <sstream>
#include <string>
#include <unordered_map>
#include <vvv3d/std/log.hpp>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvv3dgui.hpp>
#include <vvvcfg/vvvcfg.hpp>

#include "property_mapper.hpp"
#include "widget_fabric.hpp"

namespace vvv3d {
using vvv::helper::format;
namespace {

struct NameType {
    std::string name;
    std::string type;
};

/**
 * @brief Split name by last '_' symbol.
 * @param full_name */
NameType makeNameTypeFromFullName(const std::string& full_name)
{
    static const char sep = '_';
    const auto& separator_pos = full_name.rfind(sep);
    if (separator_pos == std::string::npos) {
        return {full_name, ""};
    }

    return {full_name.substr(0, separator_pos),
            full_name.substr(separator_pos + 1)};
}

std::string getType(const vvv::CfgNode::properties_type& properties,
                    const NameType& node_name)
{
    // first search defined type
    const auto& type_it = properties.find("type");
    if (type_it != properties.end())
        return type_it->second.asString();

    // try get type from name
    return node_name.type;
}

} // namespace

vvv3d::Widget* makeWidget(const vvv::CfgNode& node)
{
    using namespace std;
    using namespace vvv3d;
    using vvv::CfgNode;
    const auto& node_name = node.getName();
    const auto& name_type = makeNameTypeFromFullName(node_name);
    const auto& properties = node.getProperties();
    const auto& type = getType(properties, name_type);
    if (type.empty()) {
        LOG(format("Type of \"@\" not specified", node_name));
        return nullptr;
    }
    auto ret = WidgetFabric::instance().create(type);
    if (!ret) {
        LOG(format("Unknown widget type \"@\"", type));
        return ret;
    }
    for (const auto& prop : properties) {
        const auto& name = prop.first;
        const auto& value = prop.second;
        PropertyMapper::instance().applyProperty(ret, name, value);
    }

    if (properties.count("text") == 0) {
        const auto& text =
            properties.count("type") ? node_name : name_type.name;
        PropertyMapper::instance().applyProperty(ret, "text", text);
    }
    if (properties.count("action") == 0 && properties.count("actions") == 0) {
        const auto& action = std::string("on_") + node_name;
        PropertyMapper::instance().applyProperty(ret, "action", action);
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
    load(cfg);
}

void GuiLayer::load(const vvv::CfgNode& cfg)
{
    if (!cfg.hasChild("gui"))
        return;

    for (const auto& c : cfg.getChild("gui").getChildren()) {
        auto w = addWidgetNode(c);
        top_widgets.push_back(w);
    }
    for (auto w : topWidgets())
        addWidget(w);
}

void GuiLayer::clear()
{
    for (auto w : topWidgets()) {
        removeWidget(w);
        delete w;
    }
    top_widgets.clear();
}

GuiLayer::widget_ptr_type
GuiLayer::getByShortName(const std::string& name) const
{
    auto it = short_name_map.find(name);
    if (it != short_name_map.end())
        return it->second;
    return nullptr;
}

GuiLayer::widget_ptr_type GuiLayer::getByFullName(const std::string& name) const
{
    auto it = full_name_map.find(name);
    if (it != full_name_map.end())
        return it->second;
    return nullptr;
}

const std::vector<GuiLayer::widget_ptr_type>& GuiLayer::topWidgets() const
{
    return top_widgets;
}

void GuiLayer::setText(const std::string& widget_name, const std::string& text)
{
    auto w = get<vvv3d::ITextProperty>(widget_name);
    if (w)
        return w->setText(text);
    LOG(format("Error: \"@\" does not implement ITextProperty", widget_name));
}

std::string GuiLayer::getText(const std::string& widget_name) const
{
    auto w = get<vvv3d::ITextProperty>(widget_name);
    if (w)
        return w->getText();
    LOG(format("Error: \"@\" does not implement ITextProperty", widget_name));
    return {};
}

void GuiLayer::setColor(const std::string& widget_name,
                        const vvv3d::Color& color)
{
    auto w = get<vvv3d::IColorProperty>(widget_name);
    if (w)
        return w->setColor(color);
    LOG(format("Error: \"@\" does not implement IColorProperty", widget_name));
}
} // namespace vvv3d
