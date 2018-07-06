#include "draw.hpp"
#include <core/camera.hpp>
#include <core/graphics/fonts/textline.hpp>
#include <core/graphics/geometry.hpp>
#include <core/graphics/shaders/shader.hpp>
#include <core/graphics/textures/texture.hpp>

namespace vvv3d {

static inline void setup_shader(Shader& shader, const Camera& camera,
                                const vvv::matrix44f& model_matrix)
{
    shader.activate();
    shader.setModel(model_matrix);
    shader.setViewProjection(camera.getViewProjection());
}

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture)
{
    setup_shader(shader, camera, model_matrix);
    shader.setTexturePosition(texture.getTexturePosition());
    shader.setTexture0(texture);
    geometry.draw();
}

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1)
{
    setup_shader(shader, camera, model_matrix);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    geometry.draw();
}

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2)
{
    setup_shader(shader, camera, model_matrix);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    shader.setTexture2(texture2);
    geometry.draw();
}

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2, const Texture& texture3)
{
    setup_shader(shader, camera, model_matrix);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    shader.setTexture2(texture2);
    shader.setTexture3(texture3);
    geometry.draw();
}

void drawColored(const Camera& camera, Shader& shader, const Geometry& geometry,
                 const vvv::matrix44f& model_matrix, const Color& color0)
{
    setup_shader(shader, camera, model_matrix);
    shader.setColor0(color0);
    geometry.draw();
}

void drawTexturedColored(const Camera& camera, Shader& shader,
                         const Geometry& geometry,
                         const vvv::matrix44f& model_matrix,
                         const Texture& texture, const Color& color)
{
    setup_shader(shader, camera, model_matrix);
    shader.setTexturePosition(texture.getTexturePosition());
    shader.setTexture0(texture);
    shader.setColor0(color);
    geometry.draw();
}

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const vvv::matrix44f& model_matrix, const Font& font,
              const Color& color)
{
    setup_shader(shader, camera, model_matrix);
    const auto& tex = font.getTexture();
    shader.setTexturePosition(tex.getTexturePosition());
    shader.setTexture0(tex);
    shader.setColor0(color);
    DRAW_TRANSPARENT
    geometry.draw();
}

void scissor(int x, int y, int width, int height)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
}

vvv::vector4<int> getScissor()
{
    vvv::vector4<int> ret;
    glGetIntegerv(GL_SCISSOR_BOX, ret.vector);
    return ret;
}

void scissorDisable() { glDisable(GL_SCISSOR_TEST); }
} // namespace vvv3d
