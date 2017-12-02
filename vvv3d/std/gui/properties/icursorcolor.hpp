#ifndef VVV_ICURSORCOLOR_H
#define VVV_ICURSORCOLOR_H
#include <vvv3d/core/graphics/color.hpp>

namespace vvv3d {

class ICursorColorProperty {
public:
    virtual void setCursorColor(const Color& color) = 0;
    virtual const Color& getCursorColor() const = 0;
    virtual ~ICursorColorProperty() = default;
};

} // namespace vvv3d

#endif
