#ifndef VVV_ICOLOR_H
#define VVV_ICOLOR_H
#include <vvv3d/core/graphics/color.h>
#include <vvv3d/std/gui/widget.h>

namespace vvv3d {

class IColorProperty : public virtual Widget {
public:
    virtual void setColor(const Color& color) = 0;
    virtual const Color& getColor() const = 0;
};

} // namespace vvv3d

#endif
