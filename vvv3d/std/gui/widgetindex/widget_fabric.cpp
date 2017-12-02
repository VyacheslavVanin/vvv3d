#include "standard_widget_fabrics.hpp"
#include "widget_fabric.hpp"

namespace vvv3d {
WidgetFabric& WidgetFabric::instance()
{
    static WidgetFabric fabric;
    return fabric;
}

void WidgetFabric::addType(const std::string& name,
                           const WidgetFabric::func_t& fabric)
{
    custom_map[name] = fabric;
}

namespace {
Widget* create(const WidgetFabric::mapping& map, const std::string& name)
{
    auto it = map.find(name);
    if (it == map.end())
        return nullptr;
    return it->second();
}
} // namespace

vvv3d::Widget* WidgetFabric::create(const std::string& name) const
{
    auto ret = vvv3d::create(standard_map, name);
    if (!ret)
        ret = vvv3d::create(custom_map, name);
    return ret;
}

WidgetFabric::WidgetFabric()
    : standard_map{
          {"panel", [] { return makePanel(); }},
          {"hlayout", [] { return makeHLayout(); }},
          {"vlayout", [] { return makeVLayout(); }},
          {"button", [] { return makeTextButton(); }},
          {"label", [] { return makeLabel(); }},
          {"lineedit", [] { return makeLineEdit(); }},
          {"sliderh", [] { return makeSliderH(); }},
      }
{
}

} // namespace vvv3d