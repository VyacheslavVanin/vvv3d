#ifndef DRAW_H
#define DRAW_H

#define DRAW_TRANSPARENT for(auto trenable = [](){glDisable(GL_DEPTH_TEST); glEnable(GL_BLEND); \
                                                  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);return true;}(); \
                             trenable; \
                             glEnable(GL_DEPTH_TEST),glDisable(GL_BLEND),trenable=false)

class Camera;
class Shader;
class Geometry;
class Transform;
class Texture;
class Color;
class Font;

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Texture& texture);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Texture& texture0, const Texture& texture1);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2, const Texture& texture3);

void drawColored(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Color& color0);

void drawTexturedColored(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture, const Color& color);

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const Transform& transform, const Font& font, const Color& color);


#endif // DRAW_H
