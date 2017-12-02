#pragma once
#include <vvv3d/core/graphics/fonts/font.hpp>
#include <vvv3d/core/graphics/geometry.hpp>
#include <vvv3d/core/graphics/shaders/shader.hpp>
#include <vvv3d/core/graphics/textures/texturemanager.hpp>

namespace vvv3d {

class ResourceManager {
public:
    ResourceManager()
        : textureManager(), fontManager(), shaderManger(), geometryManager()
    {
    }

    TextureManager& getTextureManager() { return textureManager; }
    FontManager& getFontManager() { return fontManager; }
    ShaderManager& getShaderManager() { return shaderManger; };
    GeometryManager& getGeometryManager() { return geometryManager; };

private:
    TextureManager textureManager;
    FontManager fontManager;
    ShaderManager shaderManger;
    GeometryManager geometryManager;
};
}

