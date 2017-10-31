#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "font.h"
#include "mgrfreetype.h"
#include <map>
#include <vector>
#include <vvv3d/core/graphics/textures/texturepacker.h>
#include <vvv3d/utils/myutils.h>
#include <vvv3d/core/graphics/fonts/systemfonts.hpp>

namespace vvv3d {

using namespace std;
const std::u32string characters = U"abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "0123456789 ~`!@#$%^&*()_+-="
                                  "{}[]|\\/?,.<>;:'\"№"
                                  "абвгдеёжзиЙклмнопрстуфхцчшщъыьэюя"
                                  "АБВГДЕЁЖЗИйКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

void convert8to32tex(const void* in, size_t width, size_t height, void* out)
{
    const uint8_t* u8 = static_cast<const uint8_t*>(in);
    uint8_t* u32      = static_cast<uint8_t*>(out);
    const size_t size = width * height;
    for (size_t i = 0; i < size; ++i)
        for (size_t j      = 0; j < 4; ++j)
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
            const size_t inPixelIndex  = y * width + x;
            const size_t outPixelIndex = (height - y - 1) * width + x;
            for (size_t j                  = 0; j < 4; ++j)
                ret[outPixelIndex * 4 + j] = 0xff;
            ret[outPixelIndex * 4 + 3]     = u8[inPixelIndex];
        }
    return ret;
}

static std::vector<Glyph> loadGlyphes(FT_Face face,
                                      const std::u32string& characters)
{
    std::vector<Glyph> glyphes;
    for (const auto& c : characters) {
        const auto glyph_index = FT_Get_Char_Index(face, c);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        const auto glyph   = face->glyph;
        const auto& bm     = face->glyph->bitmap;
        const auto width   = bm.width;
        const auto height  = bm.rows;
        const auto xoffset = glyph->metrics.horiBearingX / 64;
        const auto yoffset = glyph->metrics.horiBearingY / 64 - height;
        const auto advance = glyph->metrics.horiAdvance / 64;
        Glyph toPlace{static_cast<uint32_t>(c),
                      static_cast<int32_t>(width),
                      static_cast<int32_t>(height),
                      static_cast<int32_t>(xoffset),
                      static_cast<int32_t>(yoffset),
                      static_cast<int32_t>(advance),
                      convert8to32tex(bm.buffer, width, height),
                      0,
                      0};
        glyphes.push_back(toPlace);
    }
    return glyphes;
}

static bool glyphSizeComparisionHeightFirst(const Glyph& l, const Glyph& r)
{
    return l.height < r.height ? true :
           l.height == r.height ? l.width < r.width
                                : false;
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
    void* ptr              = calloc(1, numPixels);

    const auto width  = static_cast<GLsizei>(lltex->getWidth());
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

struct FontImpl {
    FontImpl(FT_Face f, unsigned int size = 16, unsigned int charSize = 16,
             unsigned int dpi = 96, unsigned int textureSize = 512);
    FontImpl(const FontImpl&) = delete;
    FontImpl& operator=(const FontImpl&) = delete;

    std::map<uint32_t, Glyph> mapCharToGlyph;
    std::shared_ptr<Texture> lltex;

    FT_Face face;
    unsigned int charSize;
    unsigned int dpi;
    unsigned int pixelSize;
    unsigned int textureSize;
};

FontImpl::FontImpl(FT_Face f, unsigned int size, unsigned int charSize,
                   unsigned int dpi, unsigned int textureSize)
    : mapCharToGlyph(),
      lltex(new Texture(std::make_shared<LowLevelTexture>(
          nullptr, textureSize, textureSize, GL_RGBA, GL_RGBA))),
      face(f), charSize(charSize), dpi(dpi), pixelSize(size),
      textureSize(textureSize)
{
    FT_Set_Char_Size(face, charSize * 64, charSize * 64, dpi, dpi);
    FT_Set_Pixel_Sizes(face, 0, size);

    vector<Glyph> glyphes   = loadGlyphes(face, characters);
    vector<Glyph> notPlaced = pack2d(glyphes, textureSize, textureSize,
                                     glyphSizeComparisionHeightFirst,
                                     glyphGetSize, glyphSetOffset, glyphes, 0);

    drawGlyphesToTexture(lltex, glyphes);
    DebugVar(notPlaced.size());

    for (const auto& g : glyphes)
        mapCharToGlyph[g.character] = g;
}

void Font::activate(GLuint texUnit) { pImpl->lltex->bind(texUnit); }

const Texture& Font::getTexture() const { return *pImpl->lltex; }

long Font::getAscender() const
{
    return pImpl->face->size->metrics.ascender / 64;
}

long Font::getDescender() const
{
    return pImpl->face->size->metrics.descender / 64;
}

long Font::getHeight() const
{
    return getAscender() - getDescender();
}

long Font::getMinLeftGlyphEdge() const { return pImpl->face->bbox.xMin; }

Font::Font() : pImpl() {}

const Glyph& Font::getGlyph(uint32_t c) const
{
    return pImpl->mapCharToGlyph.at(c);
}

FontManager::FontManager() : fonts(), freetypeMgr(new MgrFreetype())
{
    const auto& sysfonts = systemFonts();
    const auto& defaultFont = sysfonts.getDefaultRegular()->getFileName();
    const auto& defaultBold = sysfonts.getDefaultBold()->getFileName();
    const auto& defaultItalic = sysfonts.getDefaultItalic()->getFileName();
    const auto& defaultMono = sysfonts.getDefaultMono()->getFileName();
    addFont("default", defaultFont, 20);
    addFont("bold", defaultBold, 20);
    addFont("italic", defaultItalic, 20);
    addFont("mono", defaultMono, 20);
}

void FontManager::addFont(const string& name, const string& filename,
                          unsigned int fontsize)
{
    FT_Face face = freetypeMgr->addFont(name, filename);
    auto f       = new Font();
    f->pImpl.reset(new FontImpl(face, fontsize, 16, 96, 256));
    fonts[name].reset(f);
}

const Font& FontManager::getFont(const string& name) const
{
    return *fonts.at(name);
}

FontManager::~FontManager() = default;
}
