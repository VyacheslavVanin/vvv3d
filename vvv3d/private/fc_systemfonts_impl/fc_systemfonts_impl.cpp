#include "fc_systemfonts_impl.hpp"

#include <algorithm>
#include <cstring>

#include <fontconfig/fontconfig.h>

namespace vvv3d {
using FontDescList = std::vector<FontDesc>;
using FontDescListIterator = FontDescList::const_iterator;

static FontDescList listSystemTTFs()
{
    FontDescList ret;
    auto config = FcInitLoadConfigAndFonts();
    auto pat = FcPatternCreate();
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

FontConfigSystemFontsImpl::FontConfigSystemFontsImpl()
    : allFonts(listSystemTTFs())
{
    loadDefaultFonts();
}

const FontDescList& FontConfigSystemFontsImpl::getFontDescList() const
{
    return allFonts;
}

} // namespace vvv3d
