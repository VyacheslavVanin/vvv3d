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

} // namespace vvv3d
