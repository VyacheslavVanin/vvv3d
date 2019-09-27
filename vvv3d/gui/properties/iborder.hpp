#pragma once

namespace vvv3d {

class IBorderProperty {
public:
    virtual void setBorder(int border) = 0;
    virtual int getBorder() const = 0;
    virtual ~IBorderProperty() = default;
};

} // namespace vvv3d
