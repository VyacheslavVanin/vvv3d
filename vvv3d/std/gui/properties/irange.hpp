#pragma once

namespace vvv3d {

class IRangeProperty {
public:
    virtual void setRange(int padding) = 0;
    virtual int getRange() const = 0;
    virtual ~IRangeProperty() = default;
};

} // namespace vvv3d
