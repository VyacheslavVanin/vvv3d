#ifndef TEXTLINE_H
#define TEXTLINE_H
#include <memory>
#include <string>
#include <vvv3d/core/graphics/color.h>
#include <vvv3d/core/graphics/fonts/font.h>
#include <vvv3d/core/graphics/geometry.h>
#include <vvv3d/core/transform.h>
#include <vvv3d/std/draw.h>

namespace vvv3d {
std::u32string toU32(const std::string& u8);
std::string toU8(const std::u32string& u32);
void updateTextGeometry(const std::shared_ptr<Geometry>& in, const Font& font,
                        const std::u32string& text);

std::shared_ptr<Geometry> createTextGeometry(const Font& font,
                                             const std::u32string& text);

std::shared_ptr<Geometry> createTextGeometry(const Font& f,
                                             const std::string& str);

void updateTextGeometry(const std::shared_ptr<Geometry>& in, const Font& font,
                        const std::string& text);
}

#endif // TEXTLINE_H
