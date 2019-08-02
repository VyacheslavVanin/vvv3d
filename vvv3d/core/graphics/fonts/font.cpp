#include "font.hpp"
#include "utils/helper.hpp"
#include <unordered_map>
#include <vector>
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/fonts/font_desc.hpp>
#include <vvv3d/core/graphics/textures/texturepacker.hpp>
#include <vvv3d/utils/myutils.hpp>

#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>

namespace vvv3d {

using namespace std;
const std::u32string characters = U"abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "0123456789 ~`!@#$%^&*()_+-="
                                  "{}[]|\\/?,.<>;:'\"№"
                                  "абвгдеёжзиЙклмнопрстуфхцчшщъыьэюя"
                                  "АБВГДЕЁЖЗИйКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

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

    std::unordered_map<uint32_t, Glyph> mapCharToGlyph;
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
Font::Font(std::unique_ptr<IFont>&& face, size_t glyph_texture_size) : pImpl()
{
    pImpl = std::make_unique<FontImpl>(std::move(face), glyph_texture_size);
}

Font::~Font() = default;

const Glyph& Font::getGlyph(uint32_t c) const { return pImpl->getGlyph(c); }

} // namespace vvv3d
