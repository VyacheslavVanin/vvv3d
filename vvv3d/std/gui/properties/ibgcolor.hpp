#pragma once
#include <vvv3d/core/graphics/color.hpp>

namespace vvv3d {

class IBGColorProperty {
public:
    virtual void setBGColor(const Color& color) = 0;
    virtual const Color& getBGColor() const = 0;
    virtual ~IBGColorProperty() = default;
};

} // namespace vvv3d
