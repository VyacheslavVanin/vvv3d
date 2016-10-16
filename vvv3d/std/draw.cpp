#include "draw.h"
#include <core/camera.h>
#include <core/graphics/textures/texture.h>
#include <core/graphics/geometry.h>
#include <core/graphics/shaders/shader.h>
#include <core/graphics/fonts/textline.h>
#include <core/transform.h>


static inline
void setup_shader(Shader& shader, const Camera& camera,
                  const Transform& transform)
{
    shader.activate();
    shader.setModel(transform.getModelMatrix());
    shader.setViewProjection(camera.getViewProjection());
}

void drawTextured(const Camera& camera, Shader& shader,
                          const Geometry& geometry,
                          const Transform& transform,
                          const Texture& texture)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture.getTexturePosition());
    shader.setTexture0(texture);
    geometry.draw();
}

void drawTextured(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture0, const Texture& texture1)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    geometry.draw();

}

void drawTextured(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture0, const Texture& texture1,
                          const Texture& texture2)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    shader.setTexture2(texture2);
    geometry.draw();
}

void drawTextured(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture0, const Texture& texture1,
                          const Texture& texture2, const Texture& texture3)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture0.getTexturePosition());
    shader.setTexture0(texture0);
    shader.setTexture1(texture1);
    shader.setTexture2(texture2);
    shader.setTexture3(texture3);
    geometry.draw();
}

void drawColored(const Camera& camera, Shader& shader,
                        const Geometry& geometry, const Transform& transform,
                        const Color& color0)
{
    setup_shader(shader, camera, transform);
    shader.setColor0(color0);
    geometry.draw();
}

void drawTexturedColored(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture, const Color& color)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture.getTexturePosition());
    shader.setTexture0(texture);
    shader.setColor0(color);
    geometry.draw();
}

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const Transform& transform, const Font& font, const Color& color)
{
    setup_shader(shader, camera, transform);
    const auto& tex = font.getTexture();
    shader.setTexturePosition(tex.getTexturePosition());
    shader.setTexture0(tex);
    shader.setColor0(color);
    DRAW_TRANSPARENT
            geometry.draw();
}
