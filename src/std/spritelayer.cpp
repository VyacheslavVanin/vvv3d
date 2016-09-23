#include <memory>
#include <std/draw.h>
#include <core/engine.h>
#include "spritelayer.h"
#include "graphics/geometry.h"


SpriteLayer::SpriteLayer() : camera(){}

void SpriteLayer::initialSetup()
{
    camera.setOrtho(-100, 100, -100, 100, -100, 100);
    auto engine = Engine::getActiveEngine();

    engine->getShaderManager().add("StdSpriteShader", "data/shaders/sprite.vsh",
                                   "data/shaders/sprite.fsh");

    static const GLfloat spriteVertices[] =
                         { -0.5, -0.5,   0, 0,
                            0.5,  0.5,   1, 1,
                           -0.5,  0.5,   0, 1,
                            0.5, -0.5,   1, 0 };
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = { 0,1,2, 0, 1, 3 };
    static const size_t numIndices =  sizeof(spriteIndices)
                                     /sizeof(spriteIndices[0]);

    using namespace std;
    auto& geomMgr = engine->getGeometryManager();
    geomMgr.add("StdSpriteGeometry",
                make_shared<Geometry>(
                    spriteVertices, sizeOfVertices,
                    spriteIndices,  numIndices,
                    VertexAttributes({ VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
                                       VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT) }),
                    GL_TRIANGLES)
                );
}

void SpriteLayer::onDraw()
{
    auto engine = Engine::getActiveEngine();
    auto& shaderMgr = engine->getShaderManager();
    auto& geomMgr = engine->getGeometryManager();
    const auto& shader = shaderMgr.get("StdSpriteShader");
    const auto& geom   = geomMgr.get("StdSpriteGeometry");

    for(auto& s:sprites)
        drawTextured(camera, *shader, *geom, s->transform, s->getTexture());
}

void SpriteLayer::onResize(int x, int y)
{
    int left = -x/2;
    int right = x + left;
    int top  = y/2;
    int bottom = top - y;
    camera.setOrtho(left, right, bottom, top, -100, 100);
}
