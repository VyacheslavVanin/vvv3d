#pragma once
#include <vvv3d/core/graphics/textures/texture.hpp>

namespace vvv3d {

class IImageProperty {
public:
    virtual void setTexture(const TextureShared& font) = 0;
    virtual const TextureShared& getTexture() const = 0;
    virtual ~IImageProperty() = default;
};

} // namespace vvv3d
