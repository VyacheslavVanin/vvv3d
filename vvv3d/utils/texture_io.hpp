#pragma once
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.hpp>
#include <vvv3d/core/graphics/textures/texture.hpp>

namespace vvv3d {
/** @brief Make low level texture from file.
 *  Currently supported file types: png, jpeg */
LowLevelTexture* readTexture(const std::string& filename);

void writeToPng(const char* filename, const LowLevelTexture* llt);
void writeToPng(const char* filename, const LowLevelTexture* llt,
                uint32_t width, uint32_t height, uint32_t offsetx,
                uint32_t offsety);
void writeToPng(const char* filename, const Texture& t);

} // namespace vvv3d
