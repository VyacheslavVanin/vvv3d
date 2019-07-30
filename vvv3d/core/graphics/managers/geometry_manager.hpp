#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/geometry.hpp>
#include <vvv3d/core/graphics/managers/base_manager.hpp>

namespace vvv3d {
class GeometryManager : public BaseManager<Geometry> {
public:
    GeometryManager();
};
} // namespace vvv3d
