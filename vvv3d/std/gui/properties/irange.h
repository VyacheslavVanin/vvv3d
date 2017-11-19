#ifndef VVV_IRANGE_H
#define VVV_IRANGE_H

namespace vvv3d {

class IRangeProperty {
public:
    virtual void setRange(int padding) = 0;
    virtual int getRange() const = 0;
    virtual ~IRangeProperty() = default;
};

} // namespace vvv3d

#endif
