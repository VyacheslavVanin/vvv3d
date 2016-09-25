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
class Colour;
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

void drawColoured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const Transform& transform,
                  const Colour& colour0);

void drawTexturedColoured(const Camera& camera, Shader& shader,
                          const Geometry& geometry, const Transform& transform,
                          const Texture& texture, const Colour& colour);

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const Transform& transform, const Font& font, const Colour& colour);


#endif // DRAW_H
