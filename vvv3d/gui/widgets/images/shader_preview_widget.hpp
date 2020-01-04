#pragma once
#include <memory>
#include <vvv3d/core/graphics/shaders/shader.hpp>
#include <vvv3d/gui/properties/iautoresize.hpp>
#include <vvv3d/gui/properties/iimage.hpp>
#include <vvv3d/gui/properties/ishader.hpp>
#include <vvv3d/gui/widgets/widget.hpp>
#include <vvv3d/sprites/sprite.hpp>

namespace vvv3d {

class ShaderPreviewWidget final : public Widget,
                                  public IShaderProperty,
                                  public IImageProperty {
public:
    explicit ShaderPreviewWidget();
    explicit ShaderPreviewWidget(const vvv3d::TextureShared& texture);
    ~ShaderPreviewWidget();

    void setTexture(const vvv3d::TextureShared& texture) override;
    const TextureShared& getTexture() const override;

    void setShader(vvv3d::ShaderShared shader) override;
    const vvv3d::ShaderShared& getShader() const override;

private:
    ShaderShared shader;
    GeometryShared geometry;
    TextureShared texture;

    // Widget interface
protected:
    void onDraw() override;
};

} // namespace vvv3d
