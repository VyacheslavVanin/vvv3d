#include "colorrectanglewidget.h"
#include <mutex>
#include <vvv3d/vvv3d.h>

namespace vvv3d {

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

static void loadSolidRectShader()
{
    const char* fsh = R"(
    #version 320 es
    precision mediump float;
    uniform vec4 color0;

    out vec4 color;

    void main(void)
    {
        color = color0;
    }
    )";

    const char* vsh = R"(
    #version 320 es
    in vec2 va_position;
    uniform mat4    viewProjectionMatrix;
    uniform vec4    position;


    void main(void)
    {
        vec2 fullPosition = position.xy + va_position*position.zw;
        gl_Position  = viewProjectionMatrix*vec4(fullPosition, 0 ,1 ) ;
    }
    )";
    auto& e  = Engine::getActiveEngine();
    auto& sm = e.getResourceManager().getShaderManager();
    sm.addFromSource("SolidRect", vsh, fsh);
}

ColorRectWidget::ColorRectWidget(const Color& color) : color(color)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& e         = Engine::getActiveEngine();
        auto& resman    = e.getResourceManager();
        auto& geomMan = resman.getGeometryManager();
        geomMan.add("SolidRect", makeRectGeometry());
        loadSolidRectShader();
    });
    setSize(60, 60);
}

void ColorRectWidget::setColor(const Color& colour) { this->color = colour; }

const Color&ColorRectWidget::getColor() const
{
    return this->color;
}

ColorRectWidget::ColorRectWidget() : ColorRectWidget(ORANGE) {}

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

}
