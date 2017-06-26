#ifndef HELPER_H
#define HELPER_H
#include <vvv3d/core/graphics/geometry.h>
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.h>

namespace vvv3d {

LowLevelTexture* readFromPng(const char* filename);
LowLevelTexture* makeDummyTexture(uint32_t width, uint32_t height,
                                  uint32_t cellSize);
Geometry* loadGeometryFrom_vvv3d(const char* filename);

#if defined ENABLE_3DS
Geometry* loadGeometryFrom3ds(const char* filename);
#endif
}

#endif // HELPER_H
