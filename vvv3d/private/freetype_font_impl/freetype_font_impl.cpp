#include "freetype_font_impl.hpp"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>
#include <vvv3d/private/freetype_font_impl/mgrfreetype.hpp>

#include <iostream>

namespace {
void convert8to32tex(const void* in, size_t width, size_t height, void* out)
{
    const uint8_t* u8 = static_cast<const uint8_t*>(in);
    uint8_t* u32 = static_cast<uint8_t*>(out);
    const size_t size = width * height;
    for (size_t i = 0; i < size; ++i)
        for (size_t j = 0; j < 4; ++j)
            u32[i * 4 + j] = (j == 3) ? u8[i] : 0xff;
}

std::vector<uint8_t> convert8to32tex(const void* in, size_t width,
                                     size_t height)
{
    const uint8_t* u8 = static_cast<const uint8_t*>(in);
    const size_t size = width * height;
    std::vector<uint8_t> ret(size * 4);

    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            const size_t inPixelIndex = y * width + x;
            const size_t outPixelIndex = (height - y - 1) * width + x;
            for (size_t j = 0; j < 4; ++j)
                ret[outPixelIndex * 4 + j] = 0xff;
            ret[outPixelIndex * 4 + 3] = u8[inPixelIndex];
        }
    return ret;
}

vvv3d::MgrFreetype& getFreetypeInstance()
{
    static vvv3d::MgrFreetype freetype_instance;
    return freetype_instance;
}

const auto FONT_MULTIPLIER = 64;

} // namespace

namespace vvv3d {
class FreetypeFont : public IFont {
public:
    FreetypeFont(FT_Face face, unsigned int size, unsigned int charSize,
                 unsigned int dpi)
        : face(face), size(size), charSize(charSize), dpi(dpi)
    {
    }

    FontMetrics GetMetrics() const override
    {
        FT_Set_Char_Size(face, 0, size * FONT_MULTIPLIER, dpi, dpi);
        // FT_Set_Pixel_Sizes(face, 0, charSize);

        FontMetrics ret;
        ret.pixelSize = size;
        ret.charSize = charSize;
        ret.dpi = dpi;
        ret.ascender = face->size->metrics.ascender / FONT_MULTIPLIER;
        ret.descender = face->size->metrics.descender / FONT_MULTIPLIER;
        ret.minLeftGlyphEdge = face->bbox.xMin;

        return ret;
    }

    Glyph GetGlyph(char32_t character) const override
    {
        const auto glyph_index = FT_Get_Char_Index(face, character);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

        if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        const auto glyph = face->glyph;
        const auto& bm = face->glyph->bitmap;
        const auto width = bm.width;
        const auto height = bm.rows;
        const auto xoffset = glyph->metrics.horiBearingX / FONT_MULTIPLIER;
        const auto yoffset =
            glyph->metrics.horiBearingY / FONT_MULTIPLIER - height;
        const auto advance = glyph->metrics.horiAdvance / FONT_MULTIPLIER;
        Glyph ret{static_cast<uint32_t>(character),
                  static_cast<int32_t>(width),
                  static_cast<int32_t>(height),
                  static_cast<int32_t>(xoffset),
                  static_cast<int32_t>(yoffset),
                  static_cast<int32_t>(advance),
                  convert8to32tex(bm.buffer, width, height),
                  0,
                  0};
        return ret;
    }

private:
    FT_Face face;
    unsigned int size;
    unsigned int charSize;
    unsigned int dpi;
};

std::unique_ptr<IFont> makeFreetypeFont(const std::string& font_file_name,
                                        unsigned int size,
                                        unsigned int charSize, unsigned int dpi)
{
    auto& freetype_instance = getFreetypeInstance();
    const auto& face = freetype_instance.addFont(font_file_name);
    return std::make_unique<FreetypeFont>(face, size, charSize, dpi);
}
} // namespace vvv3d
