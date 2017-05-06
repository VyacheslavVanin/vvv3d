#include "sprite.h"
#include <mutex>
using namespace vvv3d;

std::shared_ptr<Geometry> makeSpriteGeometry()
{
    static const GLfloat spriteVertices[] = {-0.5, -0.5, 0, 0, 0.5, 0.5,  1, 1,
                                             -0.5, 0.5,  0, 1, 0.5, -0.5, 1, 0};
    static const size_t sizeOfVertices  = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {0, 1, 2, 0, 1, 3};
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);
    return std::make_shared<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}),
        GL_TRIANGLES);
}

Sprite::Sprite() : transform(), texture(nullptr)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& resMan    = Engine::getActiveEngine().getResourceManager();
        auto& shaderMan = resMan.getShaderManager();
        auto& geomMan   = resMan.getGeometryManager();
        geomMan.add("sprite", makeSpriteGeometry());
        shaderMan.add("sprite", "data/shaders/sprite.vsh",
                      "data/shaders/sprite.fsh");
    });
}

void drawSprite(Engine& engine, const Camera& camera, const Sprite& spr)
{
    auto& resman    = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman   = resman.getGeometryManager();
    auto sh         = shaderman.get("sprite");
    auto g          = geomman.get("sprite");

    drawTextured(camera, *sh, *g, spr.transform, *spr.getTexture());
}
