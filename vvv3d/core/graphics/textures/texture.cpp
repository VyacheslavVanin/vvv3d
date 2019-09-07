#include "texture.hpp"
#include <vector>
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include "vvv3d/core/graphics/lowlevel/lowleveltexture.hpp"
#include "vvvstlhelper.hpp"

namespace vvv3d {

void Texture::bind(GLuint unit) const { tex->bind(unit); }

void Texture::bind() const { tex->bind(); }

Texture::Texture(std::shared_ptr<LowLevelTexture> tex,
                 const vvv::vector4f& texturePosition)
    : tex(tex), texturePosition(texturePosition),
      size(tex->getWidth() * texturePosition.z,
           tex->getHeight() * texturePosition.w)
{
}

Texture::Texture(LowLevelTexture* tex, const vvv::vector4f& texturePosition)
    : tex(tex), texturePosition(texturePosition),
      size(tex->getWidth() * texturePosition.z,
           tex->getHeight() * texturePosition.w)
{
}

void Texture::setMipmapFilter() { tex->setMipmapFilter(); }

TextureShared makeTextureRGBA(uint32_t width, uint32_t height)
{
    return std::make_shared<vvv3d::Texture>(makeLLTextureRGBA(width, height));
}

TextureShared makeTextureRGBA(uint32_t width, uint32_t height, uint32_t value)
{
    return std::make_shared<vvv3d::Texture>(
        makeLLTextureRGBA(width, height, value));
}

TextureShared makeTextureRGBA(uint32_t width, uint32_t height, uint8_t r,
                              uint8_t g, uint8_t b, uint8_t a)
{
    return std::make_shared<vvv3d::Texture>(
        makeLLTextureRGBA(width, height, r, g, b, a));
}

TextureShared makeTextureRGB(uint32_t width, uint32_t height)
{
    return std::make_shared<vvv3d::Texture>(makeLLTextureRGB(width, height));
}

TextureShared makeTextureRGB(uint32_t width, uint32_t height, uint32_t value)
{
    return std::make_shared<vvv3d::Texture>(
        makeLLTextureRGB(width, height, value));
}

TextureShared makeTextureRGB(uint32_t width, uint32_t height, uint8_t r,
                             uint8_t g, uint8_t b)
{
    return std::make_shared<vvv3d::Texture>(
        makeLLTextureRGB(width, height, r, g, b));
}

TextureShared makeTextureRGBAf(uint32_t width, uint32_t height)
{
    return std::make_shared<vvv3d::Texture>(makeLLTextureRGBAf(width, height));
}

TextureShared makeTextureRGBAf(uint32_t width, uint32_t height, float r,
                               float g, float b, float a)
{
    return std::make_shared<vvv3d::Texture>(
        makeLLTextureRGBAf(width, height, r, g, b, a));
}

TextureShared makeTextureDepth(uint32_t width, uint32_t height)
{
    return std::make_shared<vvv3d::Texture>(makeLLTextureDepth(width, height));
}

TextureShared makeTextureDepthf(uint32_t width, uint32_t height)
{
    return std::make_shared<vvv3d::Texture>(makeLLTextureDepthf(width, height));
}

} // namespace vvv3d
