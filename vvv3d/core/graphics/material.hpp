#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/shaders/shader.hpp>

namespace vvv3d {

class Material {
private:
    enum class PROPERTY_INDEX { DIFFUSE = 0, EMISSION, NORMAL, SPECULAR };

public:
    enum class SOURCE_TYPE { TEXTURE, COLOR, NONE };

    using ValueSources = std::unordered_map<std::string, SOURCE_TYPE>;

    static const std::string kDiffuseStr;
    static const std::string kEmissionStr;
    static const std::string kNormalStr;
    static const std::string kSpecularStr;
    static const std::vector<std::string> kAllSourcesStrings;

    Material(const Material::ValueSources& outputs,
             ShaderManager& shader_manager);
    Material(const Material::ValueSources& outputs);

    Material& setDiffuse(vvv3d::TextureShared texture);
    Material& setEmission(vvv3d::TextureShared texture);
    Material& setNormal(vvv3d::TextureShared texture);
    Material& setSpecular(vvv3d::TextureShared texture);

    Material& setDiffuse(const vvv3d::Color& color);
    Material& setEmission(const vvv3d::Color& color);
    Material& setNormal(const vvv3d::Color& color);
    Material& setSpecular(const vvv3d::Color& color);

    void activate();
    vvv3d::Shader& getShader();
    const std::vector<vvv3d::TextureShared>& getTextures() const;
    const std::vector<vvv3d::Color>& getColors() const;

private:
    Material& setTexture(PROPERTY_INDEX index, vvv3d::TextureShared&& texture);

    Material& setColor(PROPERTY_INDEX index, const vvv3d::Color& color);

    uint8_t getIndex(PROPERTY_INDEX index) const;

private:
    std::shared_ptr<vvv3d::Shader> shader;
    std::vector<uint8_t> textures_indices;
    std::vector<uint8_t> colors_indices;
    std::vector<vvv3d::TextureShared> textures;
    std::vector<vvv3d::Color> colors;
};

} // namespace vvv3d
