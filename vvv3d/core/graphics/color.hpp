#pragma once
#include <string>
#include <vector>

#include <vvv3d/vvvmath/vector4.hpp>

namespace vvv3d {

using Color = vvv::vector4f;
extern Color WHITE;
extern Color SILVER;
extern Color GRAY;
extern Color BLACK;
extern Color RED;
extern Color MAROON;
extern Color YELLOW;
extern Color OLIVE;
extern Color LIME;
extern Color GREEN;
extern Color AQUA;
extern Color TEAL;
extern Color BLUE;
extern Color NAVY;
extern Color FUCHSIA;
extern Color PURPLE;
extern Color ORANGE;

/// Convert string to Color.
/// Can parse color name such as red, white etc.
/// Can parse colors in form 0xAA8811FF or 0xAA8811
/// Can parse single float number as color (0.3 -> Color(0.3, 0.3, 0.3, 0.3))
/// if falied to parse return default color (WHITE)
Color to_color(const std::string& color_name);

/// Convert string to Color.
/// Can parse color name such as red, white etc.
/// Can parse colors in form 0xAA8811FF or 0xAA8811.
/// Can parse single float number as color (0.3 -> Color(0.3, 0.3, 0.3, 0.3))
/// if successfully parse then set p\ ok to true
/// if falied to parse return default color (WHITE) and set \p ok to false.
Color to_color(const std::string& color_name, bool& ok);
Color to_color(const std::vector<std::string>& xs);

} // namespace vvv3d
