#pragma once

namespace vvv3d {

struct FontMetrics {
    unsigned int charSize;
    unsigned int dpi;
    unsigned int pixelSize;
    unsigned int textureSize;

    int ascender;
    int descender;
    int minLeftGlyphEdge;
};

} // namespace vvv3d
