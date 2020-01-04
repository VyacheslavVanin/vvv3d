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
    gm.add(geom_name, vvv3d::makeQuadGeometry2d(2));
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

    Transform transform;
    transform.setPosition(pos.x, -pos.y, 0);
    transform.setScale(size.x, size.y, 0);
    const auto& matrix = transform.getModelMatrix();

    shader->activate();
    shader->setTime(e.timeSinceStart());
    shader->setTexturePosition(texture->getTexturePosition());
    if (texture)
        shader->setTexture0(*texture);

    shader->setModel(matrix);
    shader->setViewProjection(camera.getViewProjection());
    geometry->draw();
}

ShaderPreviewWidget::~ShaderPreviewWidget() = default;

} // namespace vvv3d
