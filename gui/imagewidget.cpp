#include "imagewidget.h"
#include <vvv3d/vvv3d.h>
#include <mutex>

static std::shared_ptr<Geometry> makeImageGeometry()
{
    static const GLfloat spriteVertices[] = {    0,   -1,   0, 0,
                                                 1,    0,   1, 1,
                                                 0,    0,   0, 1,
                                                 1,   -1,   1, 0 };
    static const size_t sizeOfVertices    = sizeof(spriteVertices);
    static const GLuint spriteIndices[]   = { 0,1,2, 0, 1, 3 };
    static const size_t numIndices        =  sizeof(spriteIndices)
                                            /sizeof(spriteIndices[0]);
    return std::make_shared<Geometry>(
                spriteVertices, sizeOfVertices,
                spriteIndices,  numIndices,
                VertexAttributes({ VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
                                   VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT) }),
                GL_TRIANGLES);
}

struct ImageWidget::ImageWidgetImpl
{
    Texture* texture = nullptr;
};

ImageWidget::ImageWidget()
    : Widget(), pImpl(std::make_unique<ImageWidgetImpl>())
{
    static std::once_flag flag;
    std::call_once(flag, [](){
        auto& e = Engine::getActiveEngine();
        auto& resman = e.getResourceManager();
        auto& shaderMan = resman.getShaderManager();
        shaderMan.add("ImageWidget", "data/shaders/guiimage.vsh",
                      "data/shaders/guiimage.fsh");

        auto& geomMan   = resman.getGeometryManager();
        geomMan.add("ImageWidget", makeImageGeometry());
    } );
}

ImageWidget::ImageWidget(Texture* texture) : ImageWidget()
{
    if(texture){
        setTexture(texture);
        setSize(texture->getWidth(), texture->getHeight());
    }
}

void ImageWidget::setTexture(Texture* texture)
{
    pImpl->texture = texture;
}

void ImageWidget::onDraw()
{
    if(pImpl->texture == nullptr){
        std::cerr << "Error: ImageWidget::Draw() without texture" <<std::endl;
        return;
    }

    auto& e = Engine::getActiveEngine();
    auto& resman = e.getResourceManager();
    auto& shaderMan = resman.getShaderManager();
    auto& geomMan = resman.getGeometryManager();
    const auto& camera = getCamera();
    auto sh = shaderMan.get("ImageWidget");
    auto geom = geomMan.get("ImageWidget");

    const auto& pos = getAbsolutePosition();
    const auto& size = getSize();
    const auto& fullPosInfo = vvv::vector4f(pos.x, -pos.y,
                                            size.x, size.y);
    const auto& texture = *pImpl->texture;

    sh->activate();
    sh->setPosition(fullPosInfo);
    sh->setTexture0(texture);
    sh->setTexturePosition(texture.getTexturePosition());
    sh->setViewProjection(camera.getViewProjection());

    geom->draw();
}

void ImageWidget::onResize(const vvv::vector2f& oldSize,
                           const vvv::vector2f& newSize)
{

}

ImageWidget::~ImageWidget() = default;
