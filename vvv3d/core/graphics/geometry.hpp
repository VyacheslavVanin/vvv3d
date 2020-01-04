#pragma once
#include "lowlevel/lowlevelgeometry.hpp"
#include <memory>
#include <unordered_map>

namespace vvv3d {

class Geometry {
public:
    Geometry(const void* vertexData, GLsizei dataSize, const GLuint* indices,
             GLsizei numIndices, const VertexAttributes& attributes);
    Geometry(const VertexAttributes& attributes);
    ~Geometry();

    void updateVB(void* data, GLsizei size);
    void updateIB(void* data, GLsizei size);
    void updateBuffers(void* vdata, GLsizei vsize, void* idata, GLsizei isize);

    void draw(GLenum mode = GL_TRIANGLES) const;

private:
    std::unique_ptr<LowLevelGeometry> geometry;
    GLsizei numIndices;
};

using GeometryShared = std::shared_ptr<Geometry>;

} // namespace vvv3d
