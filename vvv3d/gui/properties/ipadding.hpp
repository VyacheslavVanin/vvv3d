#pragma once

namespace vvv3d {

class IPaddingProperty {
public:
    virtual void setPadding(int padding) = 0;
    virtual int getPadding() const = 0;
    virtual ~IPaddingProperty() = default;
};

} // namespace vvv3d
