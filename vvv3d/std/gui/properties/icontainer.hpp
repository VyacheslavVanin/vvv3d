#pragma once
#include <vvv3d/std/gui/widget.hpp>

namespace vvv3d {

class IContainer {
public:
    virtual void addWidget(Widget* widget) = 0;
    virtual void removeWidget(Widget* widget) = 0;
    virtual ~IContainer() = default;
};

} // namespace vvv3d
