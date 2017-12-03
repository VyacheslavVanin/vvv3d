#include "color.hpp"

#include <boost/endian/conversion.hpp>
#include <iostream>
#include <unordered_map>

namespace vvv3d {
Color ORANGE = Color(1, 0.5, 0);
Color WHITE = Color(1);
Color SILVER = Color(0.75f);
Color GRAY = Color(0.5f, 0.5f, 0.5f);
Color BLACK = Color(0, 0, 0);
Color RED = Color(1, 0, 0);
Color MAROON = Color(0.5, 0, 0);
Color YELLOW = Color(1, 1, 0);
Color OLIVE = Color(0.5, 0.5, 0);
Color LIME = Color(0, 1, 0);
Color GREEN = Color(0, 0.5, 0);
Color AQUA = Color(0, 1, 1);
Color TEAL = Color(0, 0.5, 0.5);
Color BLUE = Color(0, 0, 1);
Color NAVY = Color(0, 0, 0.5);
Color FUCHSIA = Color(1, 0, 1);
Color PURPLE = Color(0.5, 0, 0.5);

namespace {
void log(const std::string& text) { std::cerr << text << "\n"; }
std::unordered_map<std::string, Color> color_name_mapping{
    {"orange", ORANGE},   {"white", WHITE},   {"silver", SILVER},
    {"gray", GRAY},       {"black", BLACK},   {"red", RED},
    {"maroon", MAROON},   {"yellow", YELLOW}, {"olive", OLIVE},
    {"lime", LIME},       {"green", GREEN},   {"aqua", AQUA},
    {"teal", TEAL},       {"blue", BLUE},     {"navy", NAVY},
    {"fuchsia", FUCHSIA}, {"purple", PURPLE}, {"grey", GRAY},
};
} // namespace

Color to_color(const std::string& value)
{
    static const auto DEFAULT_COLOR = WHITE;
    static const float unit16 = 1.0f / 0xFF;

    auto it = color_name_mapping.find(value);
    if (it != color_name_mapping.end())
        return it->second;

    const auto prefix_0x = (value.find("0x") == 0);
    const auto& len = value.size() - prefix_0x * 2;
    if (len != 6 && len != 8) {
        log("String \"" + value + "\" of len " + std::to_string(len) +
            " can't be cast to Color");
        return DEFAULT_COLOR;
    }

    size_t pos = 0;
    const auto int_value = std::stoi(value, &pos, 16);
    const auto with_alpha = (len == 8) ? int_value : int_value << 8 | 0xff;

    using boost::endian::native_to_big;
    const auto color = native_to_big(with_alpha);
    if (pos != value.size()) {
        log("String \"" + value +
            "\" contain invalid symbols, so it can't be cast to Color");
        return DEFAULT_COLOR;
    }

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&color);
    return vvv3d::Color(bytes[0] * unit16, bytes[1] * unit16, bytes[2] * unit16,
                        bytes[3] * unit16);
}

} // namespace vvv3d
