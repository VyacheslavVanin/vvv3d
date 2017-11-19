#ifndef VVV_IBGCOLOR_H
#define VVV_IBGCOLOR_H
#include <vvv3d/core/graphics/color.h>

namespace vvv3d {

class IBGColorProperty {
public:
    virtual void setBGColor(const Color& color) = 0;
    virtual const Color& getBGColor() const = 0;
    virtual ~IBGColorProperty() = default;
};

} // namespace vvv3d

#endif
