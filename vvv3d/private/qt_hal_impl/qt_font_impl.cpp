#include "qt_font_impl.h"

#include <QFontDatabase>
#include <QFontInfo>
#include <QFontMetrics>
#include <QImage>

namespace vvv3d {
QtFontImpl::QtFontImpl(const QFont& font, unsigned int font_size,
                       unsigned int pixel_size, unsigned int dpi)
    : font(font), raw_font(QRawFont::fromFont(font)), font_size(font_size),
      pixel_size(pixel_size), dpi(dpi), metrics(GetMetrics())
{
}

vvv3d::FontMetrics QtFontImpl::GetMetrics() const
{
    vvv3d::FontMetrics ret;
    ret.dpi = dpi;
    ret.charSize = font_size;
    ret.pixelSize = pixel_size;

    QFontMetrics fm(font);
    // crutch needed for compatibility with freetype impl
    static constexpr const int correction = 0;
    ret.ascender = fm.ascent() + correction;
    ret.descender = -fm.descent();
    ret.minLeftGlyphEdge = fm.minLeftBearing();

    return ret;
}

namespace {
std::vector<uint8_t> convert8to32tex(const void* in, size_t width,
                                     size_t height, size_t bytes_per_line)
{
    const uint8_t* u8 = static_cast<const uint8_t*>(in);
    const size_t size = width * height;
    std::vector<uint8_t> ret(size * 4);

    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            const size_t inPixelIndex = y * bytes_per_line + x;
            const size_t outPixelIndex = (height - y - 1) * width + x;
            for (size_t j = 0; j < 4; ++j)
                ret[outPixelIndex * 4 + j] = 0xff;
            ret[outPixelIndex * 4 + 3] = u8[inPixelIndex];
        }
    return ret;
}
} // namespace

vvv3d::Glyph QtFontImpl::GetGlyph(char32_t character) const
{
    if (!raw_font.supportsCharacter(character)) {
        const static auto placeholderChar = QString('?').toUcs4()[0];
        character = placeholderChar;
    }

    vvv3d::Glyph ret;
    QString s = QString::fromUcs4(&character, 1);
    const auto glyph_indices = raw_font.glyphIndexesForString(s);
    const auto index = glyph_indices[0];

    const auto& bbox = raw_font.boundingRect(index);
    // TODO: remove convertToFormat, change convert8to32tex
    const auto& image = raw_font.alphaMapForGlyph(index).convertToFormat(
        QImage::Format_Grayscale8);
    const auto& advances = raw_font.advancesForGlyphIndexes(glyph_indices);

    const auto bytes_per_line = image.bytesPerLine();
    const auto bytes_per_pixel = image.format() / 8;
    const auto lines =
        (bytes_per_line > 0 && bytes_per_pixel > 0)
            ? image.sizeInBytes() / bytes_per_pixel / bytes_per_line
            : 0;
    ret.width = (int)bbox.width();
    ret.height = (int)lines;
    ret.advance = (int)advances[0].x();
    ret.character = character;
    ret.xoffset = (int)bbox.left();
    ret.yoffset = (int)(-bbox.bottom());

    ret.buffer =
        convert8to32tex(image.bits(), ret.width, ret.height, bytes_per_line);

    return ret;
}
} // namespace vvv3d
