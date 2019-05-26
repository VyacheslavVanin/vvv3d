#include "systemfonts.hpp"

#include <algorithm>

namespace vvv3d {
ISystemFonts ::~ISystemFonts() = default;

using FontDescList = std::vector<FontDesc>;
using FontDescListIterator = FontDescList::const_iterator;
const FontDesc* ISystemFonts::getDesc(const char* family,
                                      const char* style) const
{
    const auto& descs = getFontDescList();
    const auto it = std::find_if(
        descs.cbegin(), descs.cend(), [&style, &family](const FontDesc& e) {
            return e.getStyle() == style && e.getFamily() == family;
        });
    if (it == descs.cend())
        return nullptr;

    return &(*it);
}

const FontDesc* ISystemFonts::loadFirstOflist(
    const std::vector<std::pair<const char*, const char*>>& list,
    const FontDesc* fallback) const
{
    for (const auto& desc : list) {
        const auto* ret = getDesc(desc.first, desc.second);
        if (ret)
            return ret;
    }
    return fallback;
}

const FontDesc* ISystemFonts::getDefaultRegular() const
{
    return defaultRegular;
}

const FontDesc* ISystemFonts::getDefaultBold() const { return defaultBold; }
const FontDesc* ISystemFonts::getDefaultItalic() const { return defaultItalic; }
const FontDesc* ISystemFonts::getDefaultMono() const { return defaultMono; }

void ISystemFonts::setDefaultRegular(const FontDesc* desc)
{
    defaultRegular = desc;
}

void ISystemFonts::setDefaultBold(const FontDesc* desc) { defaultBold = desc; }

void ISystemFonts::setDefaultItalic(const FontDesc* desc)
{
    defaultItalic = desc;
}

void ISystemFonts::setDefaultMono(const FontDesc* desc) { defaultMono = desc; }

void ISystemFonts::loadDefaultFonts()
{
    setDefaultRegular(loadFirstOflist({{"DejaVu Sans", "Book"},
                                       {"Droid Sans", "Regular"},
                                       {"Roboto", "Regular"},
                                       {"Liberation", "Regular"}}));
    setDefaultBold(loadFirstOflist({{"DejaVu Sans", "Bold"},
                                    {"Droid Sans", "Bold"},
                                    {"Roboto", "Bold"},
                                    {"Liberation Sans", "Bold"}},
                                   getDefaultRegular()));
    setDefaultItalic(loadFirstOflist({{"DejaVu Sans", "Oblique"},
                                      {"Droid Serif", "Italic"},
                                      {"Roboto", "Italic"},
                                      {"Liberation Sans", "Italic"}},
                                     getDefaultBold()));
    setDefaultMono(loadFirstOflist({{"DejaVu Sans Mono", "Book"},
                                    {"Droid Sans Mono", "Regular"},
                                    {"Liberation Mono", "Regular"}},
                                   defaultBold));
}
} // namespace vvv3d
