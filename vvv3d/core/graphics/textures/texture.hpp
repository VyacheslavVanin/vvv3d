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

    /**
     * @brief getTexturePosition Get texture position and size in atlas texture
     * @return  xy - position offset of bootom left corner (0.0 ... 1.0f),
     *          zw - width and height (0.0 ... 1.0f) */
    const vvv::vector4f& getTexturePosition() const { return texturePosition; }

    /**
     * @brief getHeight get height of texture in pixels
     * @return  */
    GLuint getHeight() const { return height; }

    /**
     * @brief getWidth get width of texture in pixels
     * @return  */
    GLuint getWidth() const { return width; }

    /**
     * @brief getFormat get OpenGL texture format ( GL_RGBA)
     * @return */
    GLint getFormat() const { return tex->getFormat(); }

    /**
     * @brief getID get Low level OpenGL texture id
     * @return Opengl texture id */
    GLuint getID() const { return tex->getID(); }

    /**
     * @brief getLowLevelTexture LowLevelTexture refferenced by this Texture
     */
    LowLevelTexture& getLowLevelTexture() const { return *tex; }

private:
    friend class TextureAtlas;
    std::shared_ptr<LowLevelTexture> tex; // pointer to real texture

    // store position in real texture (x,y)-offset, (z,w)-width,height
    vvv::vector4f texturePosition;
    GLuint width;
    GLuint height;
};

void writeToPng(const char* filename, const Texture& t);

} // namespace vvv3d
