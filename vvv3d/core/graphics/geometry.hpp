#pragma once
#include "lowlevel/lowlevelgeometry.hpp"
#include <memory>
#include <unordered_map>

namespace vvv3d {

class Geometry {
public:
    Geometry(const void* vertexData, GLsizei dataSize, const GLuint* indices,
             GLsizei numIndices, const VertexAttributes& attributes,
             GLenum mode);
    Geometry(const VertexAttributes& attributes, GLenum mode);
    ~Geometry();

    void updateVB(void* data, GLsizei size);
    void updateIB(void* data, GLsizei size);
    void updateBuffers(void* vdata, GLsizei vsize, void* idata, GLsizei isize);

    void draw() const;

private:
    std::unique_ptr<LowLevelGeometry> geometry;
    GLenum mode;
    GLsizei numIndices;
};

} // namespace vvv3d
