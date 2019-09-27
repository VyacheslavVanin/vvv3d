#include "quad.hpp"

namespace vvv3d {

std::unique_ptr<Geometry> makeQuadGeometry2d(float scale,
                                             const vvv::vector2f& pivot)
{
    const GLfloat left = (0 - pivot.x) * scale;
    const GLfloat right = (1 - pivot.x) * scale;
    const GLfloat top = (1 - pivot.y) * scale;
    const GLfloat bottom = (0 - pivot.y) * scale;
    const GLfloat spriteVertices[] = {
        left,  bottom, 0, 0, //
        right, top,    1, 1, //
        left,  top,    0, 1, //
        right, bottom, 1, 0  //
    };
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = {0, 1, 2, 0, 3, 1};
    static const size_t numIndices =
        sizeof(spriteIndices) / sizeof(spriteIndices[0]);

    return std::make_unique<Geometry>(
        spriteVertices, sizeOfVertices, spriteIndices, numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}));
}

} // namespace vvv3d
