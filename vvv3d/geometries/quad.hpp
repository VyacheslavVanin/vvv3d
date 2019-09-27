#pragma once
#include <memory>
#include <vvv3d/core/graphics/geometry.hpp>
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {

std::unique_ptr<Geometry>
makeQuadGeometry2d(float scale = 1.0f,
                   const vvv::vector2f& pivot = vvv::vector2f(0.5, 0.5));
}
