#pragma once
#include <string>
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

Color to_color(const std::string& color_name);

} // namespace vvv3d
