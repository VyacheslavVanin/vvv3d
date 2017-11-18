#ifndef VVV_ICOLOR_H
#define VVV_ICOLOR_H
#include <vvv3d/core/graphics/color.h>

namespace vvv3d {

class IColorProperty {
public:
    virtual void setColor(const Color& color) = 0;
    virtual const Color& getColor() const = 0;
    virtual ~IColorProperty() = default;
};

} // namespace vvv3d

#endif
