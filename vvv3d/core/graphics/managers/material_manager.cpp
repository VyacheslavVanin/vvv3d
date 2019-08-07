#include "material_manager.hpp"
#include <vvv3d/core/graphics/material.hpp>

namespace vvv3d {
MaterialManager::MaterialManager(ShaderManager& shm)
    : BaseManagerSafe<Material>()
{
    auto material = std::make_shared<Material>(Material::ValueSources{}, shm);
    addForce(getDefaultKey(), std::move(material));
}
} // namespace vvv3d
