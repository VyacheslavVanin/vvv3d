#ifndef LOWLEVELGEOMETRY_H
#define LOWLEVELGEOMETRY_H
#include "lowlevelbuffer.h"
#include "openglprovider.h"
#include "vertexattribute.h"
#include <memory>
#include <stdexcept>

namespace vvv3d {

class LowLevelGeometry {
public:
    LowLevelGeometry(const std::shared_ptr<LowLevelBuffer>& vb,
                     const std::shared_ptr<LowLevelBuffer>& ib,
                     const VertexAttributes& attrib);
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
    std::shared_ptr<LowLevelBuffer> vb;
    std::shared_ptr<LowLevelBuffer> ib;
    GLuint vao;
};
}

#endif // LOWLEVELGEOMETRY_H
