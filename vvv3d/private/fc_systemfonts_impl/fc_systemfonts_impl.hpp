#pragma once
#include <vvv3d/core/hal/fonts/interface/systemfonts.hpp>

namespace vvv3d {
class FontConfigSystemFontsImpl : public ISystemFonts {
public:
    FontConfigSystemFontsImpl();
    FontConfigSystemFontsImpl(const FontConfigSystemFontsImpl&) = delete;
    FontConfigSystemFontsImpl(FontConfigSystemFontsImpl&&) = delete;
    FontConfigSystemFontsImpl&
    operator=(const FontConfigSystemFontsImpl&) = delete;
    FontConfigSystemFontsImpl& operator=(FontConfigSystemFontsImpl&&) = delete;

    const std::vector<FontDesc>& getFontDescList() const;
    const FontDesc* getDefaultRegular() const;
    const FontDesc* getDefaultBold() const;
    const FontDesc* getDefaultItalic() const;
    const FontDesc* getDefaultMono() const;

private:
    std::vector<FontDesc> allFonts;
};
} // namespace vvv3d
