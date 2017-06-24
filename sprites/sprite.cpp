#include "sprite.h"
#include <mutex>
using namespace vvv3d;

std::unique_ptr<Geometry> makeSpriteGeometry()
{
    static const GLfloat spriteVertices[] = {-0.5, -0.5, 0, 0, 0.5, 0.5,  1, 1,
                                             -0.5, 0.5,  0, 1, 0.5, -0.5, 1, 0};
    static const size_t sizeOfVertices  = sizeof(spriteVertices);
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

static void loadSpriteShader()
{
    const char* fsh = R"(
    #version 330 core
    precision mediump float;
    uniform sampler2D texture0;
    uniform float time;

    in  vec2 out_position;
    in  vec2 out_texCoord;
    out vec4 color;

    void main(void)
    {
        color = texture2D( texture0, out_texCoord );
    }
    )";

    const char* vsh = R"(
    #version 330 core
    in vec2 va_position;
    in vec2 va_texCoord;
    uniform mat4    modelMatrix;
    uniform mat4    viewProjectionMatrix;
    uniform vec4    texturePosition;
    out highp vec2  out_texCoord;

    void main(void)
    {
        gl_Position  = viewProjectionMatrix*modelMatrix*vec4(va_position, 0 ,1);
        out_texCoord = texturePosition.xy + va_texCoord*texturePosition.zw;
    }
    )";
    auto& e  = Engine::getActiveEngine();
    auto& sm = e.getResourceManager().getShaderManager();
    sm.addFromSource("sprite", vsh, fsh);
}

Sprite::Sprite() : transform(), texture(nullptr)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& resMan    = Engine::getActiveEngine().getResourceManager();
        auto& geomMan   = resMan.getGeometryManager();
        geomMan.add("sprite", makeSpriteGeometry());
        loadSpriteShader();
    });
}

void drawSprite(Engine& engine, const Camera& camera, const Sprite& spr)
{
    auto& resman    = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman   = resman.getGeometryManager();
    auto& sh        = shaderman.get("sprite");
    const auto& g   = geomman.get("sprite");

    drawTextured(camera, sh, g, spr.transform, spr.getTexture());
}
