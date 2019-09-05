#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/shaders/shader.hpp>

namespace vvv3d {

class Material {
public:
    enum class SOURCE_TYPE { TEXTURE, COLOR, NONE };
    enum class PROPERTY {
        DIFFUSE = 0,
        EMISSION,
        NORMAL,
        SPECULAR,
        ROUGHNESS,
        METALLIC,
        COUNT
    };

    using ValueSources = std::unordered_map<PROPERTY, SOURCE_TYPE>;

    Material(const Material::ValueSources& outputs,
             ShaderManager& shader_manager);
    Material(const Material::ValueSources& outputs);

    Material& setDiffuse(vvv3d::TextureShared texture);
    Material& setEmission(vvv3d::TextureShared texture);
    Material& setNormal(vvv3d::TextureShared texture);
    Material& setSpecular(vvv3d::TextureShared texture);
    Material& setRoughness(vvv3d::TextureShared texture);
    Material& setMetallic(vvv3d::TextureShared texture);

    Material& setDiffuse(const vvv3d::Color& color);
    Material& setEmission(const vvv3d::Color& color);
    Material& setNormal(const vvv3d::Color& color);
    Material& setSpecular(const vvv3d::Color& color);
    // Use 'r' channel
    Material& setRoughness(const vvv3d::Color& color);
    // Use 'r' channel
    Material& setMetallic(const vvv3d::Color& color);

    void activate();
    vvv3d::Shader& getShader();
    const std::vector<vvv3d::TextureShared>& getTextures() const;
    const std::vector<vvv3d::Color>& getColors() const;

private:
    Material& setTexture(PROPERTY index, vvv3d::TextureShared&& texture);

    Material& setColor(PROPERTY index, const vvv3d::Color& color);

    uint8_t getIndex(PROPERTY index) const;

private:
    std::shared_ptr<vvv3d::Shader> shader;
    std::vector<uint8_t> textures_indices;
    std::vector<uint8_t> colors_indices;
    std::vector<vvv3d::TextureShared> textures;
    std::vector<vvv3d::Color> colors;
};

} // namespace vvv3d
