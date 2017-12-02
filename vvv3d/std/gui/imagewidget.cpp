#include "imagewidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

static std::unique_ptr<Geometry> makeImageGeometry()
{
    static const GLfloat spriteVertices[] = {0, -1, 0, 0, 1, 0,  1, 1,
                                             0, 0,  0, 1, 1, -1, 1, 0};
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {0, 1, 2, 0, 1, 3};
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);
    return std::make_unique<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}),
        GL_TRIANGLES);
}

static void loadImageShader()
{
    const char* fsh = R"(
    #version 320 es
    precision mediump float;
    uniform sampler2D texture0;
    uniform float time;

    in  vec2 out_texCoord;
    out vec4 color;

    void main(void)
    {
        color = texture(texture0, out_texCoord);
    }
    )";

    const char* vsh = R"(
    #version 320 es
    in vec2 va_position;
    in vec2 va_texCoord;
    uniform mat4    viewProjectionMatrix;
    uniform vec4    texturePosition;
    uniform vec4    position;
    out highp vec2  out_texCoord;

    void main(void)
    {
        vec2 fullPosition = position.xy + va_position*position.zw;
        gl_Position  = viewProjectionMatrix*vec4(fullPosition, 0 ,1 ) ;
        out_texCoord = texturePosition.xy + va_texCoord*texturePosition.zw;
    }
    )";
    auto& e = Engine::getActiveEngine();
    auto& sm = e.getResourceManager().getShaderManager();
    sm.addFromSource("ImageWidget", vsh, fsh);
}

ImageWidget::ImageWidget() : texture(nullptr)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        loadImageShader();

        auto& e = Engine::getActiveEngine();
        auto& resman = e.getResourceManager();
        auto& geomMan = resman.getGeometryManager();
        geomMan.add("ImageWidget", makeImageGeometry());
    });
}

ImageWidget::ImageWidget(const Texture& texture) : ImageWidget()
{
    setTexture(texture);
    setSize(texture.getWidth(), texture.getHeight());
}

void ImageWidget::setTexture(const Texture& texture)
{
    this->texture = &texture;
}

void ImageWidget::onDraw()
{
    if (texture == nullptr) {
        std::cerr << "Error: ImageWidget::Draw() without texture" << std::endl;
        return;
    }

    auto& e = Engine::getActiveEngine();
    auto& resman = e.getResourceManager();
    auto& shaderMan = resman.getShaderManager();
    auto& geomMan = resman.getGeometryManager();
    const auto& camera = getCamera();
    auto& sh = shaderMan.get("ImageWidget");
    const auto& geom = geomMan.get("ImageWidget");

    const auto& pos = getAbsolutePosition();
    const auto& size = getSize();
    const auto& fullPosInfo = vvv::vector4f(pos.x, -pos.y, size.x, size.y);
    const auto& texture = *this->texture;

    sh.activate();
    sh.setPosition(fullPosInfo);
    sh.setTexture0(texture);
    sh.setTexturePosition(texture.getTexturePosition());
    sh.setViewProjection(camera.getViewProjection());

    geom.draw();
}

ImageWidget::~ImageWidget() = default;

} // namespace vvv3d
