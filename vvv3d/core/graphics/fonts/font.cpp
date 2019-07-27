#include "font.hpp"
#include "utils/helper.hpp"
#include <map>
#include <vector>
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/fonts/font_desc.hpp>
#include <vvv3d/core/graphics/textures/texturepacker.hpp>
#include <vvv3d/core/hal/hal.hpp>
#include <vvv3d/utils/myutils.hpp>

#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>

namespace vvv3d {

namespace {
const auto CHAR_SIZE = 16;
} // namespace

using namespace std;
const std::u32string characters = U"abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "0123456789 ~`!@#$%^&*()_+-="
                                  "{}[]|\\/?,.<>;:'\"№"
                                  "абвгдеёжзиЙклмнопрстуфхцчшщъыьэюя"
                                  "АБВГДЕЁЖЗИйКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
// TODO: get correct texturesize
const constexpr size_t GLYPH_TEXURE_SIZE = 512;

static bool glyphSizeComparisionHeightFirst(const Glyph& l, const Glyph& r)
{
    return l.height < r.height
               ? true
               : l.height == r.height ? l.width < r.width : false;
}

static std::pair<uint32_t, uint32_t> glyphGetSize(const Glyph& g)
{
    return std::make_pair(g.width, g.height);
}

static void glyphSetOffset(Glyph& g, int32_t xoff, int32_t yoff, int32_t border)
{
    g.textureOffsetX = xoff + border;
    g.textureOffsetY = yoff + border;
}

static void clearTexture(std::shared_ptr<Texture> lltex)
{
    const size_t numPixels = lltex->getWidth() * lltex->getHeight() * 4;
    void* ptr = calloc(1, numPixels);

    const auto width = static_cast<GLsizei>(lltex->getWidth());
    const auto height = static_cast<GLsizei>(lltex->getWidth());
    lltex->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                    GL_UNSIGNED_BYTE, ptr);
    free(ptr);
}

static void drawGlyphesToTexture(std::shared_ptr<Texture> lltex,
                                 std::vector<Glyph>& glyphes)
{
    lltex->bind();
    clearTexture(lltex);
    for (auto& g : glyphes)
        glTexSubImage2D(GL_TEXTURE_2D, 0, g.textureOffsetX, g.textureOffsetY,
                        g.width, g.height, GL_RGBA, GL_UNSIGNED_BYTE,
                        g.buffer.data());
}

static std::vector<Glyph> loadGlyphes(const IFont& face,
                                      const std::u32string& characters)
{
    std::vector<Glyph> glyphes;
    glyphes.reserve(characters.size());
    for (const auto& c : characters)
        glyphes.push_back(face.GetGlyph(c));

    return glyphes;
}

struct FontImpl {
    FontImpl(std::unique_ptr<IFont>&& face, unsigned int textureSize = 512);
    FontImpl(const FontImpl&) = delete;
    FontImpl& operator=(const FontImpl&) = delete;

    const Glyph& getGlyph(uint32_t c) const;

    std::map<uint32_t, Glyph> mapCharToGlyph;
    std::shared_ptr<Texture> lltex;

    std::unique_ptr<IFont> face;
    FontMetrics metrics;
    Glyph dummyGlyph;
};

FontImpl::FontImpl(std::unique_ptr<IFont>&& face, unsigned int textureSize)
    : mapCharToGlyph(),
      lltex(new Texture(std::make_shared<LowLevelTexture>(
          nullptr, textureSize, textureSize, GL_RGBA, GL_RGBA))),
      face(std::move(face)), metrics(this->face->GetMetrics())
{
    vector<Glyph> glyphes = loadGlyphes(*this->face, characters);
    vector<Glyph> notPlaced = pack2d(glyphes, textureSize, textureSize,
                                     glyphSizeComparisionHeightFirst,
                                     glyphGetSize, glyphSetOffset, glyphes, 0);

    drawGlyphesToTexture(lltex, glyphes);
    if (notPlaced.size())
        DebugVar(notPlaced.size());

    for (const auto& g : glyphes)
        mapCharToGlyph[g.character] = g;

    static const std::u32string dummy = U"?";
    static const uint32_t dummyCharacter = dummy[0];
    dummyGlyph = mapCharToGlyph.at(dummyCharacter);
}

const Glyph& FontImpl::getGlyph(uint32_t c) const
{
    const auto& it = mapCharToGlyph.find(c);
    if (it == mapCharToGlyph.end())
        return dummyGlyph;

    return it->second;
}

void Font::activate(GLuint texUnit) { pImpl->lltex->bind(texUnit); }

const Texture& Font::getTexture() const { return *pImpl->lltex; }

int Font::getAscender() const { return pImpl->metrics.ascender; }

int Font::getDescender() const { return pImpl->metrics.descender; }

int Font::getHeight() const { return getAscender() - getDescender(); }

int Font::getMinLeftGlyphEdge() const
{
    return pImpl->metrics.minLeftGlyphEdge;
}

Font::Font() : pImpl() {}

Font::~Font() = default;

const Glyph& Font::getGlyph(uint32_t c) const { return pImpl->getGlyph(c); }

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

void FontManager::addFont(const string& name, const string& filename,
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

    auto f = std::make_shared<Font>(Font::_private{});
    f->pImpl = std::make_unique<FontImpl>(std::move(ifont), GLYPH_TEXURE_SIZE);
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
    auto f = std::make_shared<Font>(Font::_private{});
    f->pImpl = std::make_unique<FontImpl>(std::move(ifont), GLYPH_TEXURE_SIZE);
    fonts[name] = f;
}

const Font& FontManager::getFont(const string& name) const
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
