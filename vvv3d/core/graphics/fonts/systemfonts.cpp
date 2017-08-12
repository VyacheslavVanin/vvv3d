#include "systemfonts.hpp"
#include <algorithm>
#include <fontconfig/fontconfig.h>
#include <string.h>
#include <vector>

namespace vvv3d {

using FontDescList         = std::vector<FontDesc>;
using FontDescListIterator = FontDescList::const_iterator;

FontDesc::FontDesc(const std::string& filename, const std::string& family,
                   const std::string& style, const std::string& format)
    : filename(filename), family(family), style(style), format(format)
{
}

const std::string& FontDesc::getFileName() const { return filename; }
const std::string& FontDesc::getFamily() const { return family; }
const std::string& FontDesc::getStyle() const { return style; }
const std::string& FontDesc::getFormat() const { return format; }

static FontDescListIterator findFont(const FontDescList& descs,
                                     const std::string& style,
                                     const std::string& family)
{
    return std::find_if(
        descs.cbegin(), descs.cend(), [&style, &family](const FontDesc& e) {
            return e.getStyle() == style && e.getFamily() == family;
        });
}

static const FontDesc*
getFirstMatchingFont(const FontDescList& descs, const std::string& style,
                     const std::vector<std::string>& preferedFamilies)
{
    for (const auto& family : preferedFamilies) {
        const auto it = findFont(descs, style, family);
        if (it != descs.end())
            return &(*it);
    }

    for (const auto& font : descs)
        if (font.getStyle() == style)
            return &font;

    return nullptr;
}

static FontDescList listSystemTTFs()
{
    FontDescList ret;
    auto config = FcInitLoadConfigAndFonts();
    auto pat    = FcPatternCreate();
    auto os =
        FcObjectSetBuild(FC_FONTFORMAT, FC_FILE, FC_STYLE, FC_FAMILY, (char*)0);
    auto fs = FcFontList(config, pat, os);

    for (int i = 0; fs && i < fs->nfont; ++i) {
        FcPattern* font = fs->fonts[i];
        FcChar8 *file, *format, *style, *family;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
            FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch &&
            FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
            FcPatternGetString(font, FC_FONTFORMAT, 0, &format) ==
                FcResultMatch) {
            if (strcmp((const char*)format, "TrueType") == 0)
                ret.push_back(FontDesc((const char*)file, (const char*)family,
                                       (const char*)style,
                                       (const char*)format));
        }
    }

    FcFontSetDestroy(fs);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pat);
    FcConfigDestroy(config);
    return ret;
}

SystemFonts::SystemFonts()
    : allFonts(listSystemTTFs()),
      defaultRegular(loadFirstOflist({
            {"DejaVu Sans", "Book"},
            {"Droid Sans", "Regular"},
            {"Liberation", "Regular"}})),
      defaultBold(loadFirstOflist({
            {"DejaVu Sans", "Bold"},
            {"Droid Sans", "Bold"},
            {"Liberation Sans", "Bold"}})),
      defaultItalic(loadFirstOflist({
            {"DejaVu Sans", "Oblique"},
            {"Droid Serif", "Italic"},
            {"Liberation Sans", "Italic"}})),
      defaultMono(loadFirstOflist({
            {"DejaVu Sans Mono", "Book"},
            {"Droid Sans Mono", "Regular"},
            {"Liberation Mono", "Regular"}}))
{
}

const FontDesc* SystemFonts::getDefaultRegular() const { return defaultRegular; }
const FontDesc* SystemFonts::getDefaultBold() const { return defaultBold; }
const FontDesc* SystemFonts::getDefaultItalic() const { return defaultItalic; }
const FontDesc* SystemFonts::getDefaultMono() const { return defaultMono; }

const FontDesc* SystemFonts::loadFirstOflist(
    const std::vector<std::pair<const char*, const char*>>& list) const
{
    for (const auto& desc : list) {
        const auto* ret = getDesc(desc.first, desc.second);
        if (ret)
            return ret;
    }
    return nullptr;
}

const FontDescList& SystemFonts::getFontDescList() const { return allFonts; }

const FontDesc* SystemFonts::getDesc(const char* family,
                                     const char* style) const
{
    const auto it = findFont(allFonts, style, family);
    if (it == allFonts.end())
        return nullptr;
    return &(*it);
}

const SystemFonts& SystemFonts::instance()
{
    static const SystemFonts mgr;
    return mgr;
}

const SystemFonts& systemFonts()
{
    return SystemFonts::instance();
}
}
