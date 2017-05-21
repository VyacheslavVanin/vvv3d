#include "colorrectanglewidget.h"
#include <mutex>
#include <vvv3d/vvv3d.h>

using namespace vvv3d;

static std::unique_ptr<Geometry> makeRectGeometry()
{
    static const GLfloat spriteVertices[] = {
        0, -1, 1, 0, 0, 0, 1, -1,
    };
    static const size_t sizeOfVertices  = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {0, 1, 2, 0, 1, 3};
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);
    return std::make_unique<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT)}),
        GL_TRIANGLES);
}

ColorRectWidget::ColorRectWidget(const Color& color) : color(color)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& e         = Engine::getActiveEngine();
        auto& resman    = e.getResourceManager();
        auto& shaderMan = resman.getShaderManager();
        shaderMan.add("SolidRect", "data/shaders/solidrect.vsh",
                      "data/shaders/solidrect.fsh");

        auto& geomMan = resman.getGeometryManager();
        geomMan.add("SolidRect", makeRectGeometry());
    });
    setSize(60, 60);
}

void ColorRectWidget::setColor(const Color& colour) { this->color = colour; }

const Color&ColorRectWidget::getColor() const
{
    return this->color;
}

ColorRectWidget::ColorRectWidget() : ColorRectWidget(Color::ORANGE) {}

void ColorRectWidget::onDraw()
{
    auto& e            = Engine::getActiveEngine();
    auto& resman       = e.getResourceManager();
    auto& shaderMan    = resman.getShaderManager();
    auto& geomMan      = resman.getGeometryManager();
    const auto& camera = getCamera();
    auto& sh           = shaderMan.get("SolidRect");
    const auto& geom   = geomMan.get("SolidRect");

    const auto& pos         = getAbsolutePosition();
    const auto& size        = getSize();
    const auto& fullPosInfo = vvv::vector4f(pos.x, -pos.y, size.x, size.y);

    sh.activate();
    sh.setPosition(fullPosInfo);
    sh.setColor0(color);
    sh.setViewProjection(camera.getViewProjection());

    geom.draw();
}

ColorRectWidget::~ColorRectWidget() = default;
