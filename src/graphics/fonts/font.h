#ifndef FONT_H
#define FONT_H
#include <map>
#include <vector>
#include <memory>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <graphics/lowlevel/lowleveltexture.h>

struct Glyph
{
    uint32_t character;
    int32_t width;
    int32_t height;
    int32_t xoffset;
    int32_t yoffset;
    int32_t advance;
    std::vector<uint8_t> buffer;
    int  textureOffsetX;
    int  textureOffsetY;
};

class Font
{
private:
    Font(FT_Face f, unsigned int size=16, unsigned int charSize=16,
         unsigned int  dpi=96, unsigned int textureSize=256);

    std::map<uint32_t, std::shared_ptr<Glyph>> mapCharToGlyph;
    std::shared_ptr<LowLevelTexture> lltex;

    FT_Face face;
    unsigned int charSize;
    unsigned int dpi;
    unsigned int pixelSize;
    unsigned int textureSize;

public:
    std::shared_ptr<Glyph> getGlyph(uint32_t c) const { return mapCharToGlyph.at(c); }
    void activate(GLuint texUnit=0);
    LowLevelTexture& getTexture() {return *lltex;}
    const LowLevelTexture& getTexture() const {return *lltex;}
    long getAscender() const;
    long getDescender() const;
    long getMinLeftGlyphEdge() const;

    friend class FontManager;
};

class MgrFreetype;

class FontManager
{
public:
    FontManager();
    void addFont(const std::string& name,
                 const std::string& filename,
                 unsigned int fontsize);

    std::shared_ptr<Font> getFont(const std::string& name) const;

private:
    std::map<std::string, std::shared_ptr<Font>> fonts;
    MgrFreetype* freetypeMgr;
};



#endif // FONT_H
