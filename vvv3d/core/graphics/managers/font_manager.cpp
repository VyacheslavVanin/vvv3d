#include "font_manager.hpp"

#include "utils/helper.hpp"
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/hal/hal.hpp>

namespace vvv3d {

namespace {
const auto CHAR_SIZE = 16;
// TODO: get correct texturesize
const constexpr size_t GLYPH_TEXURE_SIZE = 512;
} // namespace

FontManager::FontManager() : fonts()
{
    bench timings("initializing font manager");
    const auto& sysfonts =
        (bench("search fonts"), vvv3d::Engine::getHAL().GetSystemFonts());
    const auto& defaultFont = *sysfonts.getDefaultRegular();
    const auto& defaultBold = *sysfonts.getDefaultBold();
    const auto& defaultItalic = *sysfonts.getDefaultItalic();
    const auto& defaultMono = *sysfonts.getDefaultMono();
    addFont("default", defaultFont, 14);
    addFont("bold", defaultBold, 14);
    addFont("italic", defaultItalic, 14);
    addFont("mono", defaultMono, 14);
}

void FontManager::addFont(const std::string& name, const std::string& filename,
                          unsigned int fontsize)
{
    bench timings("load font " + filename + " of size " +
                  std::to_string(fontsize) + " as " + name);

    if (fonts.count(name))
        return;

    const auto it = fonts.find(filename);
    if (it != fonts.end()) {
        fonts[name] = it->second;
        return;
    }

    const auto& hal = vvv3d::Engine::getHAL();
    const auto& display_props = hal.GetDisplayProperies();
    const auto& dpi = display_props.dpi.x;
    // TODO: detect or use constant pixel_size
    auto ifont = hal.GetFont(filename, fontsize, CHAR_SIZE, dpi);

    auto f = std::make_shared<Font>(Font::_private{}, std::move(ifont),
                                    GLYPH_TEXURE_SIZE);
    fonts[name] = f;
    fonts[filename] = f;
}

void FontManager::addFont(const std::string& name, const FontDesc& desc,
                          unsigned int fontsize)
{
    const auto& filename = desc.getFileName();
    if (filename.size())
        return addFont(name, filename, fontsize);

    const auto& hal = vvv3d::Engine::getHAL();
    const auto& display_props = hal.GetDisplayProperies();
    const auto& dpi = display_props.dpi.x;
    // TODO: detect or use constant pixel_size
    auto ifont =
        vvv3d::Engine::getHAL().GetFont(desc, fontsize, CHAR_SIZE, dpi);
    auto f = std::make_shared<Font>(Font::_private{}, std::move(ifont),
                                    GLYPH_TEXURE_SIZE);
    fonts[name] = f;
}

const Font& FontManager::getFont(const std::string& name) const
{
    auto it = fonts.find(name);
    if (it == fonts.end())
        return *fonts.at("default");
    return *(it->second);
}

std::vector<std::string> FontManager::listNames() const
{
    std::vector<std::string> ret;
    ret.reserve(fonts.size());
    for (const auto& font : fonts)
        ret.push_back(font.first);

    return ret;
}

FontManager::~FontManager() = default;
} // namespace vvv3d
