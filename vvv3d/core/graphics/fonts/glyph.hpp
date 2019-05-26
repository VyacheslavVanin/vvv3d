#pragma once
#include <vector>

namespace vvv3d {

struct Glyph {
    uint32_t character = {0};
    int32_t width = {0};
    int32_t height = {0};
    int32_t xoffset = {0};
    int32_t yoffset = {0};
    int32_t advance = {0};
    std::vector<uint8_t> buffer = {};
    int textureOffsetX = {0};
    int textureOffsetY = {0};
};

} // namespace vvv3d
