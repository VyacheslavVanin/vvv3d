#ifndef HELPER_H
#define HELPER_H
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.h>
#include <vvv3d/core/graphics/geometry.h>

LowLevelTexture* readFromPng(const char* filename);
Geometry* loadGeometryFrom_vvv3d(const char* filename);

#if defined ENABLE_3DS
Geometry* loadGeometryFrom3ds(const char* filename);
#endif

#endif // HELPER_H

