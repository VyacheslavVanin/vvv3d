#pragma once
#include <vvv3d/core/graphics/managers/base_manager_safe.hpp>
#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/material.hpp>

namespace vvv3d {
class MaterialManager : public BaseManagerSafe<Material> {
public:
    MaterialManager(ShaderManager& shm);
};
} // namespace vvv3d
