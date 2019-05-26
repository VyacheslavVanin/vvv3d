#pragma once
#include <memory>
#include <string>
#include <vvv3d/core/graphics/fonts/font_metrics.hpp>
#include <vvv3d/core/graphics/fonts/glyph.hpp>

namespace vvv3d {

class IFont {
public:
    virtual FontMetrics GetMetrics() const = 0;
    virtual Glyph GetGlyph(char32_t character) const = 0;
    virtual ~IFont();
};

} // namespace vvv3d
