#pragma once
#include <vvv3d/core/graphics/geometry.hpp>

namespace vvv3d {

Geometry* loadGeometryFrom_vvv3d(const char* filename);

#if defined ENABLE_3DS
Geometry* loadGeometryFrom3ds(const char* filename);
#endif
} // namespace vvv3d
