#pragma once
#include <memory>
#include <string>
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.hpp>
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {

class Texture {
public:
    /**
     * @brief Create texture view.
     * @param tex pointer to real texture
     * @param texturePosition
     *        xy - position offset of bootom left corner (0.0 ... 1.0f),
     *        zw - width and height (0.0 ... 1.0f) */
    Texture(std::shared_ptr<LowLevelTexture> tex,
            const vvv::vector4f& texturePosition = vvv::vector4f(0, 0, 1, 1));
    /**
     * @brief
     * @param tex pointer to real texture. Constructed Texture become owner of
     * LowLevelTexture referenced by tex
     * @param texturePosition */
    Texture(LowLevelTexture* tex,
            const vvv::vector4f& texturePosition = vvv::vector4f(0, 0, 1, 1));

    void bind(GLuint unit) const;
    void bind() const;
    void setMipmapFilter();

    /**
     * @brief Get texture position and size in atlas texture
     * @return  xy - position offset of bootom left corner (0.0 ... 1.0f),
     *          zw - width and height (0.0 ... 1.0f) */
    const vvv::vector4f& getTexturePosition() const { return texturePosition; }

    /**
     * @brief Get height of texture in pixels
     * @return  */
    GLuint getHeight() const { return size.y; }

    /**
     * @brief Get width of texture in pixels
     * @return  */
    GLuint getWidth() const { return size.x; }
    const vvv::vector2i& getSize() const { return size; }

    /**
     * @brief Get OpenGL texture format ( GL_RGBA)
     * @return */
    GLint getFormat() const { return tex->getFormat(); }

    /**
     * @brief Get channels count of texture (for example: RGBA -> 4, RGB -> 3)
     * @return */
    size_t getChannelsCount() const { return tex->getChannelsCount(); }
    /**
     * @brief Get Low level OpenGL texture id
     * @return Opengl texture id */
    GLuint getID() const { return tex->getID(); }

    /**
     * @brief getLowLevelTexture LowLevelTexture refferenced by this Texture */
    LowLevelTexture& getLowLevelTexture() const { return *tex; }
    std::shared_ptr<LowLevelTexture> getLowLevelTextureShared() const
    {
        return tex;
    }

private:
    friend class TextureAtlas;
    std::shared_ptr<LowLevelTexture> tex; // pointer to real texture

    /// Store position in real texture (x,y)-offset, (z,w)-width,height
    vvv::vector4f texturePosition;
    vvv::vector2i size;
};

using TextureShared = std::shared_ptr<Texture>;

TextureShared makeTextureRGBA(uint32_t width, uint32_t height);
TextureShared makeTextureRGBA(uint32_t width, uint32_t height, uint32_t value);
TextureShared makeTextureRGBA(uint32_t width, uint32_t height, uint8_t r,
                              uint8_t g, uint8_t b, uint8_t a);

TextureShared makeTextureRGB(uint32_t width, uint32_t height);
TextureShared makeTextureRGB(uint32_t width, uint32_t height, uint32_t value);
TextureShared makeTextureRGB(uint32_t width, uint32_t height, uint8_t r,
                             uint8_t g, uint8_t b);
TextureShared makeTextureRGBAf(uint32_t width, uint32_t height);
TextureShared makeTextureRGBAf(uint32_t width, uint32_t height, float r,
                               float g, float b, float a);
TextureShared makeTextureDepth(uint32_t width, uint32_t height);
TextureShared makeTextureDepthf(uint32_t width, uint32_t height);

} // namespace vvv3d
