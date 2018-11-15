#include "cube.hpp"

namespace vvv3d {
std::unique_ptr<Geometry> makeBoxGeometry(float xs, float ys, float zs,
                                          const vvv::vector3f& pivot)
{
    const GLfloat left = -xs - pivot.x;
    const GLfloat right = xs - pivot.x;
    const GLfloat top = ys - pivot.y;
    const GLfloat bottom = -ys - pivot.y;
    const GLfloat near = -zs - pivot.z;
    const GLfloat far = zs - pivot.z;
    const GLfloat spriteVertices[] = {
        right, bottom, near, -0.0, -0.0, -1.0, 0.0,  0.0,  //
        left,  top,    near, -0.0, -0.0, -1.0, 1.0,  1.0,  //
        right, top,    near, -0.0, -0.0, -1.0, 0.0,  1.0,  //
        left,  top,    far,  0.0,  0.0,  1.0,  0.0,  0.0,  //
        right, bottom, far,  0.0,  0.0,  1.0,  1.0,  1.0,  //
        right, top,    far,  0.0,  0.0,  1.0,  1.0,  -0.0, //
        right, top,    far,  1.0,  0.0,  -0.0, 1.0,  0.0,  //
        right, bottom, near, 1.0,  0.0,  -0.0, 0.0,  1.0,  //
        right, top,    near, 1.0,  0.0,  -0.0, 1.0,  1.0,  //
        right, bottom, far,  -0.0, -1.0, -0.0, 1.0,  0.0,  //
        left,  bottom, near, -0.0, -1.0, -0.0, 0.0,  1.0,  //
        right, bottom, near, -0.0, -1.0, -0.0, 1.0,  1.0,  //
        left,  bottom, near, -1.0, 0.0,  -0.0, 0.0,  0.0,  //
        left,  top,    far,  -1.0, 0.0,  -0.0, 1.0,  1.0,  //
        left,  top,    near, -1.0, 0.0,  -0.0, 1.0,  0.0,  //
        right, top,    near, 0.0,  1.0,  0.0,  0.0,  0.0,  //
        left,  top,    far,  0.0,  1.0,  0.0,  1.0,  1.0,  //
        right, top,    far,  0.0,  1.0,  0.0,  0.0,  1.0,  //
        left,  bottom, near, 0.0,  -0.0, -1.0, 1.0,  0.0,  //
        left,  bottom, far,  0.0,  -0.0, 1.0,  0.0,  1.0,  //
        right, top,    far,  1.0,  -0.0, 0.0,  1.0,  0.0,  //
        right, bottom, far,  1.0,  -0.0, 0.0,  -0.0, 0.0,  //
        right, bottom, near, 1.0,  -0.0, 0.0,  0.0,  1.0,  //
        left,  bottom, far,  -0.0, -1.0, 0.0,  -0.0, 0.0,  //
        left,  bottom, far,  -1.0, 0.0,  -0.0, 0.0,  1.0,  //
        left,  top,    near, 0.0,  1.0,  0.0,  1.0,  0.0,  //
    };
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {
        0,  1,  2,  //
        3,  4,  5,  //
        6,  7,  8,  //
        9,  10, 11, //
        12, 13, 14, //
        15, 16, 17, //
        0,  18, 1,  //
        3,  19, 4,  //
        20, 21, 22, //
        9,  23, 10, //
        12, 24, 13, //
        15, 25, 16, //
    };
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);

    return std::make_unique<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::NORMAL, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}),
        GL_TRIANGLES);
}
} // namespace vvv3d
