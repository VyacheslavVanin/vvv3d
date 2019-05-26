#pragma once
#include <map>
#include <memory>
#include <vector>
#include <vvv3d/core/graphics/fonts/font_desc.hpp>
#include <vvv3d/core/graphics/fonts/glyph.hpp>
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.hpp>
#include <vvv3d/core/graphics/textures/texture.hpp>

namespace vvv3d {

struct FontImpl;
class Font {
private:
    // part of workaround std::make_shared with private constructor
    struct _private {
    };

public:
    /// Must not fail. Must provide dummy glyph
    const Glyph& getGlyph(uint32_t c) const;
    void activate(GLuint texUnit = 0);
    const Texture& getTexture() const;
    int getAscender() const;
    int getDescender() const;
    int getMinLeftGlyphEdge() const;
    int getHeight() const;

    // part of workaround std::make_shared with private constructor
    explicit Font(const _private&) : Font() {}

    ~Font();

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
    FontManager(FontManager&&) = default;
    FontManager& operator=(FontManager&&) = default;
    ~FontManager();

    void addFont(const std::string& name, const std::string& filename,
                 unsigned int fontsize);
    void addFont(const std::string& name, const FontDesc& desc,
                 unsigned int fontsize);
    const Font& getFont(const std::string& name) const;
    std::vector<std::string> listNames() const;

private:
    std::map<std::string, std::shared_ptr<Font>> fonts;
};
} // namespace vvv3d
