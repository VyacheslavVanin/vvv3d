#include "lowleveltexture.h"


LowLevelTexture::LowLevelTexture(const void* src,
                                 GLuint imageWidth, GLuint imageHeight,
                                 GLint format, GLint internalFormat, GLenum type,
                                 GLenum target)
    :texture(~0u), target(target),
      width(imageWidth), height(imageHeight),
      internalFormat(internalFormat)
{
    glGenTextures(1,&texture);
    glBindTexture(target, texture);
    glTexImage2D(target, 0, internalFormat,
                 imageWidth, imageHeight, 0,
                 format, type, src);
    setParameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    setParameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

LowLevelTexture::LowLevelTexture(LowLevelTexture&& other) noexcept
    : texture(other.texture), target(other.target),
      width(other.width), height(other.height),
      internalFormat(other.internalFormat)
{
    other.texture = 0;
}

LowLevelTexture& LowLevelTexture::operator=(LowLevelTexture&& other) noexcept
{
    glDeleteTextures(1, &texture);
    texture = other.texture;
    target  = other.target;
    width   = other.width;
    height  = other.height;
    internalFormat = other.internalFormat;

    other.texture = 0;
    return *this;
}

LowLevelTexture::~LowLevelTexture()
{
    glDeleteTextures( 1, &texture );
}

void LowLevelTexture::bind() const
{
    glBindTexture(target, texture);
}

void LowLevelTexture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, texture);
}

void LowLevelTexture::setParameter(GLenum paramName, GLint param)
{
    this->bind();
    glTexParameteri(target, paramName, param );
}

