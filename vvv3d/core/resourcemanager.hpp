#pragma once
#include <vvv3d/core/graphics/managers/font_manager.hpp>
#include <vvv3d/core/graphics/managers/geometry_manager.hpp>
#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvv3d/core/graphics/managers/texturemanager.hpp>
#include <vvv3d/std/sprites/animation.hpp>

namespace vvv3d {

class ResourceManager {
public:
    ResourceManager()
        : textureManager(), fontManager(), shaderManger(), geometryManager(),
          animationManager()
    {
    }

    TextureManager& getTextureManager() { return textureManager; }
    FontManager& getFontManager() { return fontManager; }
    ShaderManager& getShaderManager() { return shaderManger; };
    GeometryManager& getGeometryManager() { return geometryManager; };
    AnimationManager& getAnimationManager() { return animationManager; };

private:
    TextureManager textureManager;
    FontManager fontManager;
    ShaderManager shaderManger;
    GeometryManager geometryManager;
    AnimationManager animationManager;
};
} // namespace vvv3d
