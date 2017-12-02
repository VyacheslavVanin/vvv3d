#ifndef VVV_IFONT_H
#define VVV_IFONT_H
#include <vvv3d/core/graphics/fonts/font.hpp>

namespace vvv3d {

class IFontProperty {
public:
    virtual void setFont(const Font& font) = 0;
    virtual const Font& getFont() const = 0;
    virtual ~IFontProperty() = default;
};

} // namespace vvv3d

#endif
