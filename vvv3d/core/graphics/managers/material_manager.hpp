#pragma once
#include <vvv3d/core/graphics/managers/base_manager_safe.hpp>
#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/material.hpp>
#include <vvvcfg/vvvcfg.hpp>

namespace vvv3d {
class MaterialManager : public BaseManagerSafe<Material> {
public:
    MaterialManager(ShaderManager& shm);
    void load(const vvv::CfgNode& cfg);
    void load(const std::string& cfg);
    void load(std::istream& cfg);
};
} // namespace vvv3d
