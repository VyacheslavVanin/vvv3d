#include "lowleveltexture.hpp"
#include <boost/gil/gil_all.hpp>
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include "vvv3d/core/graphics/framebufferobject.hpp"
#include <boost/gil/extension/io/png_io.hpp>

namespace vvv3d {

LowLevelTexture::LowLevelTexture(const void* src, GLuint imageWidth,
                                 GLuint imageHeight, GLint format,
                                 GLint internalFormat, GLenum type,
                                 GLenum target)
    : texture(~0u), target(target), width(imageWidth), height(imageHeight),
      internalFormat(internalFormat)
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
      height(other.height), internalFormat(other.internalFormat)
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

LowLevelTexture* readFromPng(const char* filename)
{
    using namespace boost::gil;
    image<rgba8_pixel_t> im;

    png_read_image(filename, im);

    im._view = flipped_up_down_view(im._view);

    const size_t size = im._view.size();
    const size_t numChannels = im._view.num_channels();
    const size_t width = im.dimensions().x;
    const size_t height = im.dimensions().y;

    std::vector<uint8_t> data(size * numChannels);
    uint32_t c = 0;

    for (auto p : im._view)
        for (size_t i = 0; i < numChannels; ++i)
            data[c++] = p[i];

    return new LowLevelTexture(data.data(), width, height, GL_RGBA, GL_RGBA8,
                               GL_UNSIGNED_BYTE);
}

void readImage(const LowLevelTexture* llt, void* out, GLenum format,
               GLenum type)
{
    // Do using framebuffer object instead of glGetTexImage cause on GL ES (3.2)
    // no such function
    const uint32_t width = llt->getWidth();
    const uint32_t height = llt->getHeight();
    FrameBufferObject fo;
    fo.beginDrawToColorTexture(const_cast<LowLevelTexture&>(*llt));
    glReadPixels(0, 0, width, height, format, type, out);
    fo.endDraw();
}

void writeToPng(const char* filename, const LowLevelTexture* llt)
{
    const uint32_t width = llt->getWidth();
    const uint32_t height = llt->getHeight();
    const uint32_t numChannels = 4;

    const size_t dataSize = width * height * numChannels;
    std::vector<uint8_t> data(dataSize);

    readImage(llt, data.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    using namespace boost::gil;
    png_write_view(filename,
                   flipped_up_down_view(interleaved_view(
                       width, height, (const rgba8_pixel_t*)data.data(),
                       numChannels * width)));
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
    const size_t numChannels = 4;
    const size_t size = width * height;

    std::vector<uint32_t> data(size * numChannels);
    for (size_t j = 0; j < height; ++j)
        for (size_t i = 0; i < width; ++i) {
            const size_t linear_index = j * width + i;
            data[linear_index] =
                (isWhiteCell(i, j, cellSize) * 0xffffffff) | 0xff000000;
        }

    return new LowLevelTexture(data.data(), width, height, GL_RGBA, GL_RGBA8,
                               GL_UNSIGNED_BYTE);
}

} // namespace vvv3d
