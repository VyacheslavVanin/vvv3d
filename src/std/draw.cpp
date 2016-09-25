#include "draw.h"
#include <graphics/camera.h>
#include <graphics/textures/texture.h>
#include <graphics/geometry.h>
#include <graphics/shaders/shader.h>
#include <core/transform.h>
#include <graphics/fonts/textline.h>


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

void drawColoured(const Camera& camera, Shader& shader,
                        const Geometry& geometry, const Transform& transform,
                        const Colour& colour0)
{
    setup_shader(shader, camera, transform);
    shader.setColour0(colour0);
    geometry.draw();
}

void drawTexturedColoured(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture, const Colour& colour)
{
    setup_shader(shader, camera, transform);
    shader.setTexturePosition(texture.getTexturePosition());
    shader.setTexture0(texture);
    shader.setColour0(colour);
    geometry.draw();
}

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const Transform& transform, const Font& font, const Colour& colour)
{
    setup_shader(shader, camera, transform);
    auto tex = font.getTexture();
    shader.setTexturePosition(tex->getTexturePosition());
    shader.setTexture0(*tex);
    shader.setColour0(colour);
    DRAW_TRANSPARENT
            geometry.draw();
}
