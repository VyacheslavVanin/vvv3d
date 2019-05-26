#include "texture_io.hpp"
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/textures/texturemanager.hpp>
#include <vvv3d/core/hal/hal.hpp>
#include <vvv3d/utils/helper.hpp>

namespace vvv3d {
LowLevelTexture* readTexture(const std::string& filename)
{
    bench timing(std::string("loading ") + filename);
    const auto& hal = vvv3d::Engine::getHAL();
    return hal.readTexture(filename);
}

void writeToPng(const char* filename, const LowLevelTexture* llt)
{
    writeToPng(filename, llt, llt->getWidth(), llt->getHeight(), 0, 0);
}

void writeToPng(const char* filename, const LowLevelTexture* llt,
                uint32_t width, uint32_t height, uint32_t offsetx,
                uint32_t offsety)
{
    bench timing(std::string("store ") + filename);
    const auto& hal = vvv3d::Engine::getHAL();
    hal.writeTexture(filename, llt, width, height, offsetx, offsety);
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
