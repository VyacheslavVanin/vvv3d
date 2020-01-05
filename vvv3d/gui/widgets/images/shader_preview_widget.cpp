#include "shader_preview_widget.hpp"
#include <mutex>
#include <vvv3d/geometries/quad.hpp>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

namespace {
const std::string geom_name = "shader_preview_widget";
}

ShaderPreviewWidget::ShaderPreviewWidget() : shader(), geometry()
{
    auto& gm = vvv3d::getGeometryManager();
    gm.add(geom_name, vvv3d::makeQuadGeometry2d());
    geometry = gm.getShared(geom_name);
}

ShaderPreviewWidget::ShaderPreviewWidget(const TextureShared& texture)
    : ShaderPreviewWidget()
{
    setTexture(texture);
}

void ShaderPreviewWidget::setTexture(const TextureShared& texture)
{
    this->texture = texture;
}

const TextureShared& ShaderPreviewWidget::getTexture() const { return texture; }

void ShaderPreviewWidget::setShader(vvv3d::ShaderShared shader)
{
    this->shader = std::move(shader);
}

const vvv3d::ShaderShared& ShaderPreviewWidget::getShader() const
{
    return shader;
}

void ShaderPreviewWidget::onDraw()
{
    if (!shader)
        return;

    const auto& e = vvv3d::Engine::getActiveEngine();
    const auto& camera = getCamera();

    const auto& size = getSize();
    const auto& pos = getAbsolutePosition() + size / 2;

    shader->activate();
    shader->setTime(e.timeSinceStart());
    shader->setPosition(vvv::vector4f(pos.x, -pos.y, size.x, size.y));
    if (texture) {
        shader->setTexturePosition(texture->getTexturePosition());
        shader->setTexture0(*texture);
    }

    shader->setViewProjection(camera.getViewProjection());
    geometry->draw();
}

ShaderPreviewWidget::~ShaderPreviewWidget() = default;

} // namespace vvv3d
