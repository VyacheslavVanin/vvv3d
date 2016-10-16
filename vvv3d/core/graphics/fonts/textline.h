#ifndef TEXTLINE_H
#define TEXTLINE_H
#include <string>
#include <memory>
#include <vvv3d/core/graphics/geometry.h>
#include <vvv3d/core/graphics/fonts/font.h>
#include <vvv3d/core/graphics/colour.h>
#include <vvv3d/core/transform.h>
#include <vvv3d/std/draw.h>


void updateTextGeometry(std::shared_ptr<Geometry> in,
                        const Font& font, const std::u32string& text );

std::shared_ptr<Geometry>
createTextGeometry(const Font& font, const std::u32string& text);


std::shared_ptr<Geometry>
createTextGeometry(const Font& f, const std::string& str);

void updateTextGeometry(std::shared_ptr<Geometry> in,
                        const Font& font, const std::string& text );

#endif // TEXTLINE_H
