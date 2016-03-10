#ifndef HELPER_H
#define HELPER_H
#include "graphics/lowlevel/lowleveltexture.h"
#include "graphics/geometry.h"

LowLevelTexture* readFromPng(const char* filename);
Geometry* loadGeometryFrom3ds(const char* filename);
Geometry* loadGeometryFrom_vvv3d(const char* filename);

#endif // HELPER_H

