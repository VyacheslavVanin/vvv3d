#ifndef FONT_H
#define FONT_H
#include <map>
#include <memory>
#include <vector>
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.h>
#include <vvv3d/core/graphics/textures/texture.h>

namespace vvv3d {

struct Glyph {
    uint32_t character          = {0};
    int32_t width               = {0};
    int32_t height              = {0};
    int32_t xoffset             = {0};
    int32_t yoffset             = {0};
    int32_t advance             = {0};
    std::vector<uint8_t> buffer = {};
    int textureOffsetX          = {0};
    int textureOffsetY          = {0};
};

class MgrFreetype;
struct FontImpl;
class Font {
public:
    const Glyph& getGlyph(uint32_t c) const;
    void activate(GLuint texUnit = 0);
    const Texture& getTexture() const;
    int getAscender() const;
    int getDescender() const;
    int getMinLeftGlyphEdge() const;
    int getHeight() const;

private:
    Font();
    friend class FontManager;
    std::unique_ptr<FontImpl> pImpl;
};

class FontManager {
public:
    FontManager();
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&)                 = default;
    FontManager& operator=(FontManager&&) = default;
    ~FontManager();

    void addFont(const std::string& name, const std::string& filename,
                 unsigned int fontsize);
    const Font& getFont(const std::string& name) const;

private:
    std::map<std::string, std::unique_ptr<Font>> fonts;
    std::unique_ptr<MgrFreetype> freetypeMgr;
};
}

#endif // FONT_H
