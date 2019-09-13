#include "geometry.hpp"
#include "lowlevel/lowlevelbuffer.hpp"
#include "lowlevel/vertexattribute.hpp"

using namespace vvv3d;

Geometry::Geometry(const void* vertexData, GLsizei dataSize,
                   const GLuint* indices, GLsizei numIndices,
                   const VertexAttributes& attributes)
    : geometry(new LowLevelGeometry(
          LowLevelBuffer(GL_ARRAY_BUFFER, vertexData, dataSize),
          LowLevelBuffer(GL_ELEMENT_ARRAY_BUFFER, indices,
                         numIndices * sizeof(GLuint)),
          attributes)),
      numIndices(numIndices)
{
}

Geometry::Geometry(const VertexAttributes& attributes)
    : geometry(new LowLevelGeometry(
          LowLevelBuffer(GL_ARRAY_BUFFER, 0, 0),
          LowLevelBuffer(GL_ELEMENT_ARRAY_BUFFER, 0, 0), attributes)),
      numIndices(0)
{
}

Geometry::~Geometry() {}

void Geometry::updateVB(void* data, GLsizei size)
{
    geometry->setVertexBufferData(data, size);
}

void Geometry::updateIB(void* data, GLsizei size)
{
    geometry->setIndexBufferData(data, size);
    numIndices = size / sizeof(GLint);
}

void Geometry::updateBuffers(void* vdata, GLsizei vsize, void* idata,
                             GLsizei isize)
{
    geometry->setBuffersData(vdata, vsize, idata, isize);
    numIndices = isize / sizeof(GLint);
}

void Geometry::draw(GLenum mode) const { geometry->Draw(mode, numIndices); }
