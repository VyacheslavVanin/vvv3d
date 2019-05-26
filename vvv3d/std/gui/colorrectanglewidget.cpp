#include "colorrectanglewidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

static std::unique_ptr<Geometry> makeRectGeometry()
{
    static const GLfloat spriteVertices[] = {
        0, -1, 1, 0, 0, 0, 1, -1,
    };
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {0, 1, 2, 0, 3, 1};
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);
    return std::make_unique<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT)}),
        GL_TRIANGLES);
}

static std::unique_ptr<vvv3d::Shader> loadSolidRectShader()
{
    static const char* fsh = R"(
    #version 300 es
    precision mediump float;
    uniform vec4 color0;

    out vec4 color;

    void main(void)
    {
        color = color0;
    }
    )";

    static const char* vsh = R"(
    #version 300 es
    in vec2 va_position;
    uniform mat4    viewProjectionMatrix;
    uniform vec4    position;


    void main(void)
    {
        vec2 fullPosition = position.xy + va_position*position.zw;
        gl_Position  = viewProjectionMatrix*vec4(fullPosition, 0 ,1 ) ;
    }
    )";
    return vvv3d::Shader::fromStrings("SolidRect", vsh, fsh);
}

ColorRectWidget::ColorRectWidget(const Color& color) : color(color)
{
    loadResources(); // TODO: need more centralized aproach to init resources
    setSize(60, 60);
}

void ColorRectWidget::setColor(const Color& colour) { this->color = colour; }

const Color& ColorRectWidget::getColor() const { return this->color; }

ColorRectWidget::ColorRectWidget() : ColorRectWidget(ORANGE) {}

void ColorRectWidget::onDraw()
{
    auto& shaderMan = getShaderManager();
    auto& geomMan = getGeometryManager();
    const auto& camera = getCamera();
    auto& sh = shaderMan.get("SolidRect");
    const auto& geom = geomMan.get("SolidRect");

    const auto& pos = getAbsolutePosition();
    const auto& size = getSize();
    const auto& fullPosInfo = vvv::vector4f(pos.x, -pos.y, size.x, size.y);

    sh.activate();
    sh.setPosition(fullPosInfo);
    sh.setColor0(color);
    sh.setViewProjection(camera.getViewProjection());

    geom.draw();
}

void ColorRectWidget::loadResources()
{
    auto& geomMan = getGeometryManager();
    geomMan.add("SolidRect", makeRectGeometry);

    auto& shaderMan = getShaderManager();
    shaderMan.add("SolidRect", loadSolidRectShader);
}

ColorRectWidget::~ColorRectWidget() = default;

} // namespace vvv3d
