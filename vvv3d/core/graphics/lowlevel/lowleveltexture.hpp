#pragma once
#include "openglprovider.hpp"
#include <string>

namespace vvv3d {

class LowLevelTexture {
public:
    LowLevelTexture() = default;
    LowLevelTexture(const void* src, GLuint imageWidth, GLuint imageHeight,
                    GLint format, GLint internalFormat,
                    GLenum type = GL_UNSIGNED_BYTE,
                    GLenum target = GL_TEXTURE_2D);
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
    size_t getChannelsCount() const { return num_channels; }
    void setMipmapFilter();

private:
    GLuint texture;
    GLenum target;
    GLuint width;
    GLuint height;
    GLint internalFormat;
    size_t num_channels;
};

LowLevelTexture* makeDummyTexture(uint32_t width, uint32_t height,
                                  uint32_t cellSize);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_RGBA,
 *                            GL_RGBA, GL_UNSIGNED_BYTE);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height);
LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height,
                                   uint32_t value);
LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height, uint8_t r,
                                   uint8_t g, uint8_t b, uint8_t a);

LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height);
LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height,
                                  uint32_t value);
LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height, uint8_t r,
                                  uint8_t g, uint8_t b);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_RGBA,
 *                            GL_RGBA32F, GL_FLOAT);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureRGBAf(uint32_t width, uint32_t height);
LowLevelTexture* makeLLTextureRGBAf(uint32_t width, uint32_t height, float r,
                                    float g, float b, float a);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_DEPTH_COMPONENT,
 *                            GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureDepth(uint32_t width, uint32_t height);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_DEPTH_COMPONENT,
 *                            GL_DEPTH_COMPONENT32F, GL_FLOAT);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureDepthf(uint32_t width, uint32_t height);

/**
 * @brief Copy data from texture llt to out
 * @param llt pointer to texture to copy from
 * @param out pointer to destination buffer */
void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type);
void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type, uint32_t offsetx, uint32_t offsety, uint32_t width,
               uint32_t height);
} // namespace vvv3d
