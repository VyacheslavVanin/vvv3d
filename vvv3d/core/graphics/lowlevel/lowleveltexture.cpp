#include "lowleveltexture.hpp"
#include <boost/gil/gil_all.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION >= 106800
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <fstream>
#else
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#endif
#include "utils/helper.hpp"
#include "vvv3d/core/graphics/framebufferobject.hpp"

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

namespace {
LowLevelTexture*
toLowLevelTexture(const boost::gil::image<boost::gil::rgba8_pixel_t>& im)
{
    const size_t size = im._view.size();
    const size_t numChannels = im._view.num_channels();
    const size_t width = im.dimensions().x;
    const size_t height = im.dimensions().y;

    std::vector<uint8_t> data(size * numChannels);
    uint32_t c = 0;

    for (const auto& p : im._view)
        for (size_t i = 0; i < numChannels; ++i)
            data[c++] = p[i];

    return new LowLevelTexture(data.data(), width, height, GL_RGBA, GL_RGBA8,
                               GL_UNSIGNED_BYTE);
}

#if BOOST_VERSION >= 106800
namespace {
template <typename TAG, typename TI>
void read_image(const char* filename, TI& image)
{
    namespace bg = boost::gil;
    std::ifstream stream(filename, std::ios::binary);
    bg::image_read_settings<TAG> read_settings;
    bg::read_and_convert_image(stream, image, read_settings);
}
} // namespace
#endif

template <typename T>
void png_write(const char* filename, const T& view)
{
#if BOOST_VERSION < 106800
    png_write_view(filename, view);
#else
    std::ofstream stream(filename, std::ios::binary);

    namespace bg = boost::gil;
    bg::image_write_info<bg::png_tag> write_info;
    bg::write_view(stream, view, write_info);
#endif
}
} // namespace

LowLevelTexture* readFromPng(const char* filename)
{
    using namespace boost::gil;
    image<rgba8_pixel_t> im;

#if BOOST_VERSION < 106800
    png_read_and_convert_image(filename, im);
#else
    read_image<boost::gil::png_tag>(filename, im);
#endif

    im._view = flipped_up_down_view(im._view);
    return toLowLevelTexture(im);
}

LowLevelTexture* readFromJpeg(const char* filename)
{
    using namespace boost::gil;
    image<rgba8_pixel_t> im;

#if BOOST_VERSION < 106800
    jpeg_read_and_convert_image(filename, im);
#else
    read_image<boost::gil::jpeg_tag>(filename, im);
#endif

    im._view = flipped_up_down_view(im._view);
    return toLowLevelTexture(im);
}

namespace {
std::string getExtension(const std::string& filename)
{
    const auto pos = filename.rfind('.');
    if (pos == std::string::npos)
        return {};
    return filename.substr(pos + 1);
}
} // namespace

LowLevelTexture* makeLLTexture(const std::string& filename)
{
    bench timing(std::string("loading ") + filename);

    const auto& ext = getExtension(filename);
    if (ext == "png")
        return readFromPng(filename.c_str());
    if (ext == "jpeg" || ext == "jpg")
        return readFromJpeg(filename.c_str());
    throw std::invalid_argument("Unsuported image format");
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
    auto view = flipped_up_down_view(interleaved_view(
        width, height, (const rgba8_pixel_t*)data.data(), numChannels * width));
    png_write(filename, view);
}

void writeToPng(const char* filename, const LowLevelTexture* llt,
                uint32_t width, uint32_t height, uint32_t offsetx,
                uint32_t offsety)
{
    const uint32_t wholeWidth = llt->getWidth();
    const uint32_t wholeHeight = llt->getHeight();
    const uint32_t numChannels = 4;

    const size_t dataSize = wholeWidth * wholeHeight * numChannels;
    std::vector<uint8_t> data(dataSize);

    readImage(llt, data.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    size_t i = 0;
    for (size_t y = offsety; y < offsety + height; ++y) {
        for (size_t x = offsetx; x < offsetx + width; ++x) {
            data[i++] = data[y * wholeWidth * 4 + x * 4];
            data[i++] = data[y * wholeWidth * 4 + x * 4 + 1];
            data[i++] = data[y * wholeWidth * 4 + x * 4 + 2];
            data[i++] = data[y * wholeWidth * 4 + x * 4 + 3];
        }
    }
    data.resize(i);

    using namespace boost::gil;
    auto view = interleaved_view(
        width, height, (const rgba8_pixel_t*)data.data(), numChannels * width);
    view = flipped_up_down_view(view);
    png_write(filename, view);
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

} // namespace vvv3d
