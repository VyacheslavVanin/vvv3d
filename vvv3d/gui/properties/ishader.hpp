#pragma once
#include <vvv3d/core/graphics/shaders/shader.hpp>

namespace vvv3d {

class IShaderProperty {
public:
    virtual void setShader(ShaderShared shader) = 0;
    virtual const ShaderShared& getShader() const = 0;
    virtual ~IShaderProperty() = default;
};

} // namespace vvv3d
