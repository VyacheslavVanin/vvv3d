#pragma once
#include <functional>
#include <unordered_map>

#include <vvv3d/gui/widgets/widget.hpp>

namespace vvv3d {
class WidgetFabric {

public:
    using func_t = std::function<Widget*()>;
    using mapping = std::unordered_map<std::string, func_t>;

    static WidgetFabric& instance();
    void addType(const std::string& name, const func_t& fabric);
    Widget* create(const std::string& name) const;

private:
    WidgetFabric();
    WidgetFabric(const WidgetFabric&) = delete;
    WidgetFabric(WidgetFabric&&) = delete;
    WidgetFabric& operator=(const WidgetFabric&) = delete;
    WidgetFabric& operator=(WidgetFabric&&) = delete;

    mapping standard_map;
    mapping custom_map;
};
} // namespace vvv3d
