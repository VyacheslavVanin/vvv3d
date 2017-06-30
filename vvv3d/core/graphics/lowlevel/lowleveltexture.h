#ifndef LOWLEVELTEXTURE_H
#define LOWLEVELTEXTURE_H
#include "openglprovider.h"

namespace vvv3d {

class LowLevelTexture {
public:
    LowLevelTexture() = default;
    LowLevelTexture(const void* src, GLuint imageWidth, GLuint imageHeight,
                    GLint format, GLint internalFormat,
                    GLenum type             = GL_UNSIGNED_BYTE,
                    GLenum target           = GL_TEXTURE_2D);
    LowLevelTexture(const LowLevelTexture&) = delete;
    LowLevelTexture& operator=(const LowLevelTexture&) = delete;
    LowLevelTexture(LowLevelTexture&& other) noexcept;
    LowLevelTexture& operator=(LowLevelTexture&& other) noexcept;
    ~LowLevelTexture();

    void bind() const;
    void bind(GLuint unit) const;
    void setParameter(GLenum paramName, GLint param);
    GLuint getWidth() const { return width; }
    GLuint getHeight() const { return height; }

    GLuint getID() const { return texture; }
    GLint getFormat() const { return internalFormat; }

private:
    GLuint texture;
    GLenum target;
    GLuint width;
    GLuint height;
    GLint internalFormat;
};

LowLevelTexture* readFromPng(const char* filename);
void writeToPng(const char* filename, const LowLevelTexture* llt);
LowLevelTexture* makeDummyTexture(uint32_t width, uint32_t height,
                                  uint32_t cellSize);
}

#endif // LOWLWVWLTEXTURE_H
