#pragma once
#include <memory>
#include <vvv3d/core/graphics/geometry.hpp>
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {

/**
 * @brief Create box
 * @param xs - half size of x side
 * @param ys - half size of y side
 * @param zs - half size of z side
 * @param pivot
 * @return Geometry */
std::unique_ptr<Geometry>
makeBoxGeometry(float xs, float ys, float zs,
                const vvv::vector3f& pivot = vvv::vector3f(0.0, 0.0, 0.0));
} // namespace vvv3d
