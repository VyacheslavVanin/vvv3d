#include "color.hpp"

#include <unordered_map>

#include <vvv3d/std/log.hpp>

namespace vvv3d {
using vvv::helper::format;
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
        LOG(format("String \"@\" of len @ can't be cast to Color", value, len));
        return DEFAULT_COLOR;
    }

    size_t pos = 0;
    const auto int_value = std::stoul(value, &pos, 16);
    if (pos != value.size()) {
        LOG(format("String \"@\" contain invalid symbols, so it can't be cast "
                   "to Color",
                   value));
        return DEFAULT_COLOR;
    }
    const auto with_alpha =
        (len == 8ul) ? int_value : int_value << 8ul | 0xfful;

    // rgba
    const auto alpha = with_alpha & 0xfful;
    const auto blue = (with_alpha >> 8ul) & 0xfful;
    const auto green = (with_alpha >> 16ul) & 0xfful;
    const auto red = with_alpha >> 24ul;

    return vvv3d::Color(red * unit16, green * unit16, blue * unit16,
                        alpha * unit16);
}

} // namespace vvv3d
