#ifndef VVV_IVALUE_H
#define VVV_IVALUE_H

namespace vvv3d {

class IValueProperty {
public:
    virtual void setValue(int padding) = 0;
    virtual int getValue() const = 0;
    virtual ~IValueProperty() = default;
};

} // namespace vvv3d

#endif
