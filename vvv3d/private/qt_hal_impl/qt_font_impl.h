#ifndef QT_FONT_IMPL_H
#define QT_FONT_IMPL_H
#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>

#include <QFont>
#include <QRawFont>

namespace vvv3d {
class QtFontImpl : public vvv3d::IFont {
public:
    QtFontImpl(const QFont& font, unsigned int font_size,
               unsigned int pixel_size, unsigned int dpi);

    // IFont interface
    vvv3d::FontMetrics GetMetrics() const override;
    vvv3d::Glyph GetGlyph(char32_t character) const override;

private:
    QFont font;
    QRawFont raw_font;
    unsigned int font_size = 0;
    unsigned int pixel_size = 0;
    unsigned int dpi = 0;
    vvv3d::FontMetrics metrics;
};

} // namespace vvv3d

#endif // QT_FONT_IMPL_H
