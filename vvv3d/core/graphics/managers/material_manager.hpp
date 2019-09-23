#pragma once
#include <vvv3d/core/graphics/managers/base_manager_safe.hpp>
#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/material.hpp>
#include <vvvcfg/vvvcfg.hpp>

namespace vvv3d {
class MaterialManager : public BaseManagerSafe<Material> {
public:
    MaterialManager(ShaderManager& shm);

    /// Config example:
    /// materials:
    ///   plastic diffuse=gray metallic=0.1 roughness=0.8
    ///   red_plastic $materials.plastic diffuse=red
    ///   color_plastic $materials.plastic diffuse="0xDD7705"
    ///   some_cool_texture \
    ///     diffuse="path/to/textures/color.png" \           # color map
    ///     normal="path/to/textures/normal_map.png" \
    ///     metallic="path/to/textures/metallic_map.png" \   # single channel
    ///     emission="path/to/textures/emission_map.png" \   # color map
    ///     roughness="path/to/textures/roughness_map.png"   # single channel
    void load(const vvv::CfgNode& cfg);
    void load(const std::string& cfg);
    void load(std::istream& cfg);
};
} // namespace vvv3d
