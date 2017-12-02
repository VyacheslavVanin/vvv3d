#pragma once
#include "lowlevelbuffer.hpp"
#include "openglprovider.hpp"
#include "vertexattribute.hpp"
#include <memory>
#include <stdexcept>

namespace vvv3d {

class LowLevelGeometry {
public:
    LowLevelGeometry(LowLevelBuffer&& vb, LowLevelBuffer&& ib,
                     const VertexAttributes& attrib) noexcept;
    LowLevelGeometry(const LowLevelGeometry&) = delete;
    LowLevelGeometry& operator=(const LowLevelGeometry&) = delete;
    LowLevelGeometry(LowLevelGeometry&& other) noexcept;
    LowLevelGeometry& operator=(LowLevelGeometry&& other) noexcept;

    ~LowLevelGeometry();

    void Draw(GLenum mode, GLsizei count) const;

    void setVertexBufferData(const void* data, GLsizei size);

    void setIndexBufferData(const void* data, GLsizei size);

    void setBuffersData(const void* vdata, GLsizei vsize, const void* idata,
                        GLsizei isize);

private:
    void bindVAO() const;

    void freeResources();
    LowLevelBuffer vb;
    LowLevelBuffer ib;
    GLuint vao;
};
} // namespace vvv3d
