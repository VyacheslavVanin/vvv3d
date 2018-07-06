#include "sprite.hpp"
#include <mutex>

#include "vvv3d/std/geometries/quad.hpp"

namespace vvv3d {

static void loadSpriteShader()
{
    const char* fsh = R"(
    #version 300 es
    precision mediump float;
    uniform sampler2D texture0;
    uniform float time;

    in  vec2 out_position;
    in  vec2 out_texCoord;
    out vec4 color;

    void main(void)
    {
        color = texture(texture0, out_texCoord);
    }
    )";

    const char* vsh = R"(
    #version 300 es
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
    auto& sm = getShaderManager();
    sm.addFromSource("sprite", vsh, fsh);
}

Sprite::Sprite() : transform(), texture(nullptr)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& geomMan = getGeometryManager();
        geomMan.add("sprite", makeQuadGeometry2d());
        loadSpriteShader();
    });
}

const vvv3d::Texture& Sprite::getTexture() const
{
    if (!texture)
        return getTextureManager().getDefault();
    return *texture;
}

void drawSprite(const Camera& camera, const Sprite& spr)
{
    auto& shaderman = getShaderManager();
    auto& geomman = getGeometryManager();
    auto& sh = shaderman.get("sprite");
    const auto& g = geomman.get("sprite");

    const auto& model_matrix = spr.transform.getModelMatrix();
    drawTextured(camera, sh, g, model_matrix, spr.getTexture());
}
} // namespace vvv3d
