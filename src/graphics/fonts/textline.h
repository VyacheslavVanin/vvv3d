#ifndef TEXTLINE_H
#define TEXTLINE_H
#include <string>
#include <memory>
#include <graphics/geometry.h>
#include <graphics/fonts/font.h>
#include <graphics/colour.h>
#include <core/transform.h>
#include <std/draw.h>


void updateTextGeometry(std::shared_ptr<Geometry> in,
                        const Font& font, const std::u32string& text );

std::shared_ptr<Geometry>
createTextGeometry(const Font& font, const std::u32string& text);

std::shared_ptr<Geometry>
createTextGeometry(const Font& f, const std::string& str);

std::shared_ptr<Geometry>
createTextGeometry(const Font& f, std::u32string&& str);



#endif // TEXTLINE_H
