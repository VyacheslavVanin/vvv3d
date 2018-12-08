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

private:
    GLuint texture;
    GLenum target;
    GLuint width;
    GLuint height;
    GLint internalFormat;
};

void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type);

/** @brief Make low level texture from file.
 *  Currently supported file types: png, jpeg */
LowLevelTexture* makeLLTexture(const std::string& filename);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_RGBA,
 *                            GL_RGBA, GL_UNSIGNED_BYTE);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height);

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
 * new vvv3d::LowLevelTexture(0, width, height, GL_RGBA,
 *                            GL_RGBA32F, GL_FLOAT);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureRGBAf(uint32_t width, uint32_t height);

/**
 * @brief Create texture to store color data
 * same as
 * new vvv3d::LowLevelTexture(0, width, height, GL_DEPTH_COMPONENT,
 *                            GL_DEPTH_COMPONENT32F, GL_FLOAT);
 * should be freed after use with delete or passed to owner object
 * (Texture, TextureManager) */
LowLevelTexture* makeLLTextureDepthf(uint32_t width, uint32_t height);

void writeToPng(const char* filename, const LowLevelTexture* llt);
void writeToPng(const char* filename, const LowLevelTexture* llt,
                uint32_t width, uint32_t height, uint32_t offsetx,
                uint32_t offsety);
LowLevelTexture* makeDummyTexture(uint32_t width, uint32_t height,
                                  uint32_t cellSize);
} // namespace vvv3d
