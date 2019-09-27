#pragma once

namespace vvv3d {

class IAutoResizeProperty {
public:
    virtual void setAutoResize(bool resize) = 0;
    virtual bool isAutoResize() const = 0;
    virtual ~IAutoResizeProperty() = default;
};

} // namespace vvv3d
