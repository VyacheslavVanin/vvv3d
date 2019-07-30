#pragma once
#include <map>
#include <memory>
#include <vector>
#include <vvv3d/core/graphics/fonts/glyph.hpp>
#include <vvv3d/core/graphics/textures/texture.hpp>

namespace vvv3d {

struct IFont;
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
    explicit Font(const _private&, std::unique_ptr<IFont>&& face,
                  size_t glyph_texture_size)
        : Font(std::move(face), glyph_texture_size)
    {
    }

    ~Font();

private:
    Font();
    Font(std::unique_ptr<IFont>&& face, size_t glyph_texture_size);
    friend class FontManager;
    std::unique_ptr<FontImpl> pImpl;
};

} // namespace vvv3d
