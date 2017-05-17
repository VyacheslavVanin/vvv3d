#include "texture.h"

using namespace vvv3d;

void Texture::bind(GLuint unit) const { tex->bind(unit); }

void Texture::bind() const { tex->bind(); }

Texture::Texture(const std::shared_ptr<LowLevelTexture>& tex,
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
