#include "cube.hpp"
#include <iostream>

namespace vvv3d {
struct vertex_t {
    vvv::vector3f position;
    vvv::vector3f normal;
    vvv::vector3f tangent;
    vvv::vector3f bitangent;
    vvv::vector2f tex_coord;
};
namespace {
using triangle_verts_refs = std::tuple<vertex_t&, vertex_t&, vertex_t&>;

triangle_verts_refs getTriangleVertices(std::vector<vertex_t>& vertices,
                                        const std::vector<GLuint>& indices,
                                        size_t triangle_index)
{
    const size_t i = triangle_index * 3;
    if (i >= indices.size())
        throw std::out_of_range("triangle_index out of range");
    const auto i0 = indices[i];
    const auto i1 = indices[i + 1];
    const auto i2 = indices[i + 2];

    return std::tie(vertices[i0], vertices[i1], vertices[i2]);
}

void calculateTangents(triangle_verts_refs& triangle)
{
    auto& v1 = std::get<0>(triangle);
    auto& v2 = std::get<1>(triangle);
    auto& v3 = std::get<2>(triangle);
    const auto& edge1 = v2.position - v1.position;
    const auto& edge2 = v3.position - v1.position;
    const auto& deltaUV1 = v2.tex_coord - v1.tex_coord;
    const auto& deltaUV2 = v3.tex_coord - v1.tex_coord;

    const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    v1.tangent
        .set(f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
             f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
             f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z))
        .normalize();
    v2.tangent = v1.tangent;
    v3.tangent = v1.tangent;

    v1.bitangent
        .set(f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
             f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
             f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z))
        .normalize();
    v2.bitangent = v1.bitangent;
    v3.bitangent = v1.bitangent;
}

void calculateTangents(std::vector<vertex_t>& vertices,
                       const std::vector<GLuint>& indices)
{
    const auto& triangles_count = indices.size() / 3;
    for (size_t i = 0; i < triangles_count; ++i) {
        auto triangle = getTriangleVertices(vertices, indices, i);
        calculateTangents(triangle);
    }
}

} // namespace

std::unique_ptr<Geometry> makeBoxGeometry(float xs, float ys, float zs,
                                          const vvv::vector3f& pivot)
{
    const GLfloat left = -xs - pivot.x;
    const GLfloat right = xs - pivot.x;
    const GLfloat top = ys - pivot.y;
    const GLfloat bottom = -ys - pivot.y;
    const GLfloat near = -zs - pivot.z;
    const GLfloat far = zs - pivot.z;
    std::vector<vertex_t> spriteVertices = {
        {{right, bottom, near}, {0.0, 0.0, -1.0}, {}, {}, {0.0, 0.0}}, //
        {{left, top, near}, {0.0, 0.0, -1.0}, {}, {}, {1.0, 1.0}},     //
        {{right, top, near}, {0.0, 0.0, -1.0}, {}, {}, {0.0, 1.0}},    //
        {{left, top, far}, {0.0, 0.0, 1.0}, {}, {}, {0.0, 0.0}},       //
        {{right, bottom, far}, {0.0, 0.0, 1.0}, {}, {}, {1.0, 1.0}},   //
        {{right, top, far}, {0.0, 0.0, 1.0}, {}, {}, {1.0, 0.0}},      //
        {{right, top, far}, {1.0, 0.0, 0.0}, {}, {}, {1.0, 0.0}},      //
        {{right, bottom, near}, {1.0, 0.0, 0.0}, {}, {}, {0.0, 1.0}},  //
        {{right, top, near}, {1.0, 0.0, 0.0}, {}, {}, {1.0, 1.0}},     //
        {{right, bottom, far}, {0.0, -1.0, 0.0}, {}, {}, {1.0, 0.0}},  //
        {{left, bottom, near}, {0.0, -1.0, 0.0}, {}, {}, {0.0, 1.0}},  //
        {{right, bottom, near}, {0.0, -1.0, 0.0}, {}, {}, {1.0, 1.0}}, //
        {{left, bottom, near}, {-1.0, 0.0, 0.0}, {}, {}, {0.0, 0.0}},  //
        {{left, top, far}, {-1.0, 0.0, 0.0}, {}, {}, {1.0, 1.0}},      //
        {{left, top, near}, {-1.0, 0.0, 0.0}, {}, {}, {1.0, 0.0}},     //
        {{right, top, near}, {0.0, 1.0, 0.0}, {}, {}, {0.0, 0.0}},     //
        {{left, top, far}, {0.0, 1.0, 0.0}, {}, {}, {1.0, 1.0}},       //
        {{right, top, far}, {0.0, 1.0, 0.0}, {}, {}, {0.0, 1.0}},      //
        {{left, bottom, near}, {0.0, 0.0, -1.0}, {}, {}, {1.0, 0.0}},  //
        {{left, bottom, far}, {0.0, 0.0, 1.0}, {}, {}, {0.0, 1.0}},    //
        {{right, bottom, far}, {1.0, 0.0, 0.0}, {}, {}, {0.0, 0.0}},   //
        {{left, bottom, far}, {0.0, -1.0, 0.0}, {}, {}, {0.0, 0.0}},   //
        {{left, bottom, far}, {-1.0, 0.0, 0.0}, {}, {}, {0.0, 1.0}},   //
        {{left, top, near}, {0.0, 1.0, 0.0}, {}, {}, {1.0, 0.0}},      //
    };
    const size_t sizeOfVertices = spriteVertices.size() * sizeof(vertex_t);
    static const std::vector<GLuint> spriteIndices = {
        0,  1,  2,  //
        3,  4,  5,  //
        6,  7,  8,  //
        9,  10, 11, //
        12, 13, 14, //
        15, 16, 17, //
        0,  18, 1,  //
        3,  19, 4,  //
        6,  20, 7,  //
        9,  21, 10, //
        12, 22, 13, //
        15, 23, 16, //
    };
    static const size_t numIndices = spriteIndices.size();
    calculateTangents(spriteVertices, spriteIndices);

    return std::make_unique<Geometry>(
        spriteVertices.data(), sizeOfVertices, spriteIndices.data(), numIndices,
        VertexAttributes(
            {VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::NORMAL, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TANGENT, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::BITANGENT, 3, GL_FLOAT),
             VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}));
}
} // namespace vvv3d
