#ifndef DRAW_H
#define DRAW_H
class Camera;
class Shader;
class Geometry;
class Transform;
class Texture;
class Colour;

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


#endif // DRAW_H
