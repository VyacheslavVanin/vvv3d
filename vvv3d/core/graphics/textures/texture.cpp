#include "texture.hpp"
#include <boost/gil/gil_all.hpp>
#include <vector>
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include "vvv3d/core/graphics/lowlevel/lowleveltexture.hpp"
#include "vvvstlhelper.hpp"
#include <boost/gil/extension/io/png_io.hpp>
#include <iostream>

namespace vvv3d {

void Texture::bind(GLuint unit) const { tex->bind(unit); }

void Texture::bind() const { tex->bind(); }

Texture::Texture(std::shared_ptr<LowLevelTexture> tex,
                 const vvv::vector4f& texturePosition)
    : tex(tex), texturePosition(texturePosition),
      width(tex->getWidth() * texturePosition.z),
      height(tex->getHeight() * texturePosition.w)
{
}

Texture::Texture(LowLevelTexture* tex, const vvv::vector4f& texturePosition)
    : tex(tex), texturePosition(texturePosition),
      width(tex->getWidth() * texturePosition.z),
      height(tex->getHeight() * texturePosition.w)
{
}

void writeToPng(const char* filename, const Texture& t)
{
    const LowLevelTexture* llt = &t.getLowLevelTexture();

    const uint32_t wholeWidth = llt->getWidth();
    const uint32_t wholeHeight = llt->getHeight();
    const uint32_t width = t.getWidth();
    const uint32_t height = t.getHeight();
    const uint32_t offsetx = t.getTexturePosition().x * wholeWidth;
    const uint32_t offsety = t.getTexturePosition().y * wholeHeight;
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
    png_write_view(filename, view);
}

} // namespace vvv3d
