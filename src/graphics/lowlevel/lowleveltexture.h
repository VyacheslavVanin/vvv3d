#ifndef LOWLEVELTEXTURE_H
#define LOWLEVELTEXTURE_H
#include "openglprovider.h"

class LowLevelTexture
{
public:
    LowLevelTexture() = default;
    LowLevelTexture(const void* src,
                    GLuint imageWidth, GLuint  imageHeight,
                    GLint format,      GLint   internalFormat, GLenum type=GL_UNSIGNED_BYTE,
                    GLenum target=GL_TEXTURE_2D);
    ~LowLevelTexture();

    void bind() const;
    void bind(GLuint unit) const;
    void setParameter(GLenum paramName, GLint param);
    GLuint getWidth() const {return width;}
    GLuint getHeight() const {return height;}

    GLuint getID() const {return texture;}
    GLint  getFormat() const {return internalFormat;}

private:
    GLuint texture;
    GLenum target;
    GLuint width;
    GLuint height;
    GLint internalFormat;
};

#endif // LOWLWVWLTEXTURE_H
