#pragma once
#include <vector>

#include <vvv3d/core/graphics/fonts/font_desc.hpp>

namespace vvv3d {
class ISystemFonts {
public:
    virtual ~ISystemFonts();

    virtual const std::vector<FontDesc>& getFontDescList() const = 0;

    const FontDesc* getDefaultRegular() const;
    const FontDesc* getDefaultBold() const;
    const FontDesc* getDefaultItalic() const;
    const FontDesc* getDefaultMono() const;

    void setDefaultRegular(const FontDesc* desc);
    void setDefaultBold(const FontDesc* desc);
    void setDefaultItalic(const FontDesc* desc);
    void setDefaultMono(const FontDesc* desc);

    void loadDefaultFonts();
    const FontDesc* getDesc(const char* family, const char* style) const;
    const FontDesc* loadFirstOflist(
        const std::vector<std::pair<const char*, const char*>>& list,
        const FontDesc* fallback = nullptr) const;

private:
    const FontDesc* defaultRegular;
    const FontDesc* defaultBold;
    const FontDesc* defaultItalic;
    const FontDesc* defaultMono;
};

} // namespace vvv3d
