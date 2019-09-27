#pragma once
#include <vvv3d/gui/widgets/widget.hpp>

namespace vvv3d {

class IContainer {
public:
    virtual void addWidget(Widget* widget) = 0;
    virtual void removeWidget(Widget* widget) = 0;
    virtual ~IContainer() = default;
};

} // namespace vvv3d
