#ifndef HELPER_H
#define HELPER_H
#include "core/graphics/lowlevel/lowleveltexture.h"
#include "core/graphics/geometry.h"

LowLevelTexture* readFromPng(const char* filename);
Geometry* loadGeometryFrom3ds(const char* filename);
Geometry* loadGeometryFrom_vvv3d(const char* filename);

#endif // HELPER_H

