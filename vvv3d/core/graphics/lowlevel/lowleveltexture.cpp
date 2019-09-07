#include "lowleveltexture.hpp"

#include <unordered_map>

#include "utils/helper.hpp"
#include "vvv3d/core/graphics/framebufferobject.hpp"

namespace vvv3d {

size_t getChannelsCount(GLint format)
{
    const static std::unordered_map<GLenum, size_t> mapping = {
        {GL_RGB, 3},
        {GL_RGBA, 4},
        {GL_RED, 1},
        {GL_DEPTH_COMPONENT, 1},
    };
    const auto it = mapping.find(format);
    if (it == mapping.end())
        throw std::logic_error("getChannelsCount() do not support format " +
                               std::to_string(format));

    return it->second;
}

LowLevelTexture::LowLevelTexture(const void* src, GLuint imageWidth,
                                 GLuint imageHeight, GLint format,
                                 GLint internalFormat, GLenum type,
                                 GLenum target)
    : texture(~0u), target(target), width(imageWidth), height(imageHeight),
      internalFormat(internalFormat),
      num_channels(vvv3d::getChannelsCount(format))
{
    glGenTextures(1, &texture);
    glBindTexture(target, texture);
    glTexImage2D(target, 0, internalFormat, imageWidth, imageHeight, 0, format,
                 type, src);
    setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

LowLevelTexture::LowLevelTexture(LowLevelTexture&& other) noexcept
    : texture(other.texture), target(other.target), width(other.width),
      height(other.height), internalFormat(other.internalFormat),
      num_channels(other.num_channels)
{
    other.texture = 0;
}

LowLevelTexture& LowLevelTexture::operator=(LowLevelTexture&& other) noexcept
{
    glDeleteTextures(1, &texture);
    texture = other.texture;
    target = other.target;
    width = other.width;
    height = other.height;
    internalFormat = other.internalFormat;
    num_channels = other.num_channels;

    other.texture = 0;
    return *this;
}

LowLevelTexture::~LowLevelTexture() { glDeleteTextures(1, &texture); }

void LowLevelTexture::bind() const { glBindTexture(target, texture); }

void LowLevelTexture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, texture);
}

void LowLevelTexture::setParameter(GLenum paramName, GLint param)
{
    this->bind();
    glTexParameteri(target, paramName, param);
}

void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type)
{
    // Do using framebuffer object instead of glGetTexImage cause on GL ES (3.2)
    // no such function
    const uint32_t width = llt->getWidth();
    const uint32_t height = llt->getHeight();
    readImage(llt, out, format, type, 0, 0, width, height);
}

void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type, uint32_t offsetx, uint32_t offsety, uint32_t width,
               uint32_t height)
{
    // Do using framebuffer object instead of glGetTexImage cause on GL ES (3.2)
    // no such function
    FrameBufferObject fo;
    fo.beginDrawToColorTexture(const_cast<LowLevelTexture&>(*llt));
    glReadPixels(offsetx, offsety, width, height, format, type, out);
    fo.endDraw();
}

static bool isWhiteCell(uint32_t x, uint32_t y, uint32_t cellSize)
{
    const size_t ix = (x / cellSize) % 2;
    const size_t iy = (y / cellSize) % 2;
    return ix == iy;
}

LowLevelTexture* makeDummyTexture(uint32_t width, uint32_t height,
                                  uint32_t cellSize)
{
    bench timing(std::string("make dummy texture ") + std::to_string(width) +
                 "x" + std::to_string(height));
    const size_t size = width * height;

    std::vector<uint32_t> data(size);
    for (size_t j = 0; j < height; ++j)
        for (size_t i = 0; i < width; ++i) {
            const size_t linear_index = j * width + i;
            data[linear_index] =
                isWhiteCell(i, j, cellSize) ? 0xffffffff : 0xff000000;
        }

    return new LowLevelTexture(data.data(), width, height, GL_RGBA, GL_RGBA8,
                               GL_UNSIGNED_BYTE);
}

LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height)
{
    return new vvv3d::LowLevelTexture(0, width, height, GL_RGB, GL_RGB8,
                                      GL_UNSIGNED_BYTE);
}

LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height,
                                  uint32_t value)
{
    const uint8_t r = (value & 0x00FF0000u) >> 16u;
    const uint8_t g = (value & 0x00FF00u) >> 8u;
    const uint8_t b = value & 0xFFu;
    return makeLLTextureRGB(width, height, r, g, b);
}

LowLevelTexture* makeLLTextureRGB(uint32_t width, uint32_t height, uint8_t r,
                                  uint8_t g, uint8_t b)
{
    static const constexpr size_t num_channels = 3;
    const size_t size = width * height * num_channels;
    std::vector<uint8_t> data(size);
    for (size_t i = 0; i < size; i += num_channels) {
        data[i] = r;
        data[i + 1] = g;
        data[i + 2] = b;
    }

    return new LowLevelTexture(data.data(), width, height, GL_RGB, GL_RGB8,
                               GL_UNSIGNED_BYTE);
}

LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height)
{
    return new vvv3d::LowLevelTexture(0, width, height, GL_RGBA, GL_RGBA8,
                                      GL_UNSIGNED_BYTE);
}

LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height,
                                   uint32_t value)
{
    const size_t size = width * height;
    std::vector<uint32_t> data(size, value);
    return new LowLevelTexture(data.data(), width, height, GL_RGBA, GL_RGBA8,
                               GL_UNSIGNED_BYTE);
}

LowLevelTexture* makeLLTextureRGBA(uint32_t width, uint32_t height, uint8_t r,
                                   uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t value = 0;
    value |= a;
    value |= b << 8u;
    value |= g << 16u;
    value |= r << 24u;
    return makeLLTextureRGBA(width, height, value);
}

LowLevelTexture* makeLLTextureDepth(uint32_t width, uint32_t height)
{
    return new vvv3d::LowLevelTexture(0, width, height, GL_DEPTH_COMPONENT,
                                      GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
}
LowLevelTexture* makeLLTextureRGBAf(uint32_t width, uint32_t height)
{
    return new vvv3d::LowLevelTexture(0, width, height, GL_RGBA, GL_RGBA32F,
                                      GL_FLOAT);
}

LowLevelTexture* makeLLTextureRGBAf(uint32_t width, uint32_t height, float r,
                                    float g, float b, float a)
{
    static const constexpr size_t num_channels = 4;
    const size_t size = width * height * num_channels;
    std::vector<float> data(size);
    for (size_t i = 0; i < size; i += 4) {
        data[i] = r;
        data[i + 1] = g;
        data[i + 2] = b;
        data[i + 3] = a;
    }
    return new vvv3d::LowLevelTexture(data.data(), width, height, GL_RGBA,
                                      GL_RGBA32F, GL_FLOAT);
}

LowLevelTexture* makeLLTextureDepthf(uint32_t width, uint32_t height)
{
    return new vvv3d::LowLevelTexture(0, width, height, GL_DEPTH_COMPONENT,
                                      GL_DEPTH_COMPONENT32F, GL_FLOAT);
}

} // namespace vvv3d
