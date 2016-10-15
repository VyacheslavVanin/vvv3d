#include "colorrectanglewidget.h"
#include <vvv3d/vvv3d.h>
#include <mutex>

static std::shared_ptr<Geometry> makeRectGeometry()
{
    static const GLfloat spriteVertices[] = {    0,   -1,
                                                 1,    0,
                                                 0,    0,
                                                 1,   -1,};
    static const size_t sizeOfVertices    = sizeof(spriteVertices);
    static const GLuint spriteIndices[]   = { 0,1,2, 0, 1, 3 };
    static const size_t numIndices        =  sizeof(spriteIndices)
                                            /sizeof(spriteIndices[0]);
    return std::make_shared<Geometry>(
                spriteVertices, sizeOfVertices,
                spriteIndices,  numIndices,
                VertexAttributes({VertexAttribDesc(ATTRIB_LOCATION::POSITION,
                                                   2, GL_FLOAT)}),
                GL_TRIANGLES);
}

struct ColorRectangleWidget::ColorRectangleWidgetImpl
{
    ColorRectangleWidgetImpl(const Colour& colour = Colour::ORANGE)
        : colour(colour)
    {}
    Colour colour;
};


ColorRectangleWidget::ColorRectangleWidget(const Colour& colour)
    : pImpl(std::make_unique<ColorRectangleWidgetImpl>(colour))
{
    static std::once_flag flag;
    std::call_once(flag, [](){
        auto& e = Engine::getActiveEngine();
        auto& resman = e.getResourceManager();
        auto& shaderMan = resman.getShaderManager();
        shaderMan.add("SolidRect", "data/shaders/solidrect.vsh",
                      "data/shaders/solidrect.fsh");

        auto& geomMan   = resman.getGeometryManager();
        geomMan.add("SolidRect", makeRectGeometry());
    } );
    setSize(60, 60);
}

ColorRectangleWidget::ColorRectangleWidget()
    : ColorRectangleWidget(Colour::ORANGE)
{}

void ColorRectangleWidget::onDraw()
{
    auto& e = Engine::getActiveEngine();
    auto& resman = e.getResourceManager();
    auto& shaderMan = resman.getShaderManager();
    auto& geomMan = resman.getGeometryManager();
    const auto& camera = getCamera();
    auto sh = shaderMan.get("SolidRect");
    auto geom = geomMan.get("SolidRect");

    const auto& pos = getAbsolutePosition();
    const auto& size = getSize();
    const auto& fullPosInfo = vvv::vector4f(pos.x, -pos.y,
                                            size.x, size.y);

    sh->activate();
    sh->setPosition(fullPosInfo);
    sh->setColour0(pImpl->colour);
    sh->setViewProjection(camera.getViewProjection());

    geom->draw();
}

ColorRectangleWidget::~ColorRectangleWidget() = default;
