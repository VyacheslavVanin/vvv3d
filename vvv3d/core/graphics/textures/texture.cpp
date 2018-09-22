#include "texture.hpp"
#include <vector>
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include "vvv3d/core/graphics/lowlevel/lowleveltexture.hpp"
#include "vvvstlhelper.hpp"
#include <iostream>

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

void writeToPng(const char* filename, const Texture& t)
{
    const LowLevelTexture* llt = &t.getLowLevelTexture();
    const uint32_t wholeWidth = llt->getWidth();
    const uint32_t wholeHeight = llt->getHeight();
    const uint32_t width = t.getWidth();
    const uint32_t height = t.getHeight();
    const uint32_t offsetx = t.getTexturePosition().x * wholeWidth;
    const uint32_t offsety = t.getTexturePosition().y * wholeHeight;
    writeToPng(filename, llt, width, height, offsetx, offsety);
}

} // namespace vvv3d
