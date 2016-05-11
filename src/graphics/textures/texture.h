#ifndef TEXTURE_H
#define TEXTURE_H
#include <memory>
#include <string>
#include <graphics/lowlevel/lowleveltexture.h>
#include "vvvmath/linalg.h"

class Texture
{
public:
    Texture(const std::shared_ptr<LowLevelTexture>& tex,
            const vvv::vector4f& texturePosition=vvv::vector4f(0,0,1,1));

    void bind(GLuint unit) const;
    void bind() const;

    /**
     * @brief getTexturePosition Get texture position and size in atlas texture
     * @return  xy - position offset of bootom left corner (0.0 ... 1.0f),
     *          zw - width and height (0.0 ... 1.0f) */
    const vvv::vector4f &getTexturePosition() const { return texturePosition; }

    /**
     * @brief getHeight get height of texture in pixels
     * @return  */
    GLuint getHeight() const                        { return height;}

    /**
     * @brief getWidth get width of texture in pixels
     * @return  */
    GLuint getWidth() const                         { return width;}

    /**
     * @brief getFormat get OpenGL texture format ( GL_RGBA)
     * @return */
    GLint  getFormat() const                        { return tex->getFormat();}

    /**
     * @brief getID get Low level OpenGL texture id
     * @return Opengl texture id */
    GLuint getID() const                            { return tex->getID(); }

private:
    std::shared_ptr<LowLevelTexture> tex;// pointer to real texture
    vvv::vector4f texturePosition;       //store position in real texture (x,y)-offset, (z,w)-width,height
    GLuint width;
    GLuint height;
};


#endif // TEXTURE_H
