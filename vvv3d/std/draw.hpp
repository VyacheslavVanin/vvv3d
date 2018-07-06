#pragma once
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/vvvmath/vector4.hpp>

namespace vvv3d {

#define DRAW_TRANSPARENT                                                       \
    for (auto trenable =                                                       \
             []() {                                                            \
                 glDisable(GL_DEPTH_TEST);                                     \
                 glEnable(GL_BLEND);                                           \
                 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);            \
                 return true;                                                  \
             }();                                                              \
         trenable;                                                             \
         glEnable(GL_DEPTH_TEST), glDisable(GL_BLEND), trenable = false)

class Camera;
class Shader;
class Geometry;
class Texture;
class Font;

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2);

void drawTextured(const Camera& camera, Shader& shader,
                  const Geometry& geometry, const vvv::matrix44f& model_matrix,
                  const Texture& texture0, const Texture& texture1,
                  const Texture& texture2, const Texture& texture3);

void drawColored(const Camera& camera, Shader& shader, const Geometry& geometry,
                 const vvv::matrix44f& model_matrix, const Color& color0);

void drawTexturedColored(const Camera& camera, Shader& shader,
                         const Geometry& geometry,
                         const vvv::matrix44f& model_matrix,
                         const Texture& texture, const Color& color);

void drawText(const Camera& camera, Shader& shader, const Geometry& geometry,
              const vvv::matrix44f& model_matrix, const Font& font,
              const Color& color);

void scissor(int x, int y, int width, int height);
void scissorDisable();
vvv::vector4<int> getScissor();
} // namespace vvv3d
