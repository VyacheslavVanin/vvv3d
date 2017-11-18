#include "lowlevelgeometry.h"

using namespace vvv3d;

LowLevelGeometry::LowLevelGeometry(LowLevelBuffer&& vb, LowLevelBuffer&& ib,
                                   const VertexAttributes& attrib) noexcept
    : vb(std::move(vb)), ib(std::move(ib)), vao(0)
{
    glGenVertexArrays(1, &vao);
    bindVAO();
    this->vb.bind();
    this->ib.bind();
    attrib.enable();
}

LowLevelGeometry::LowLevelGeometry(LowLevelGeometry&& other) noexcept
    : vb(std::move(other.vb)), ib(std::move(other.ib)), vao(other.vao)
{
    other.vao = 0;
}

LowLevelGeometry& LowLevelGeometry::operator=(LowLevelGeometry&& other) noexcept
{
    vb = std::move(other.vb);
    ib = std::move(other.ib);
    freeResources();
    vao = other.vao;
    other.vao = 0;
    return *this;
}

LowLevelGeometry::~LowLevelGeometry() { freeResources(); }

void LowLevelGeometry::Draw(GLenum mode, GLsizei count) const
{
    static GLuint activeVAO = 0;
    if (activeVAO == vao)
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    else {
        activeVAO = vao;
        bindVAO();
        ib.bind(); // WORKAROUND: needed due to bug in some drivers
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    }
}

void LowLevelGeometry::setVertexBufferData(const void* data, GLsizei size)
{
    bindVAO();
    vb.setData(data, size);
}

void LowLevelGeometry::setIndexBufferData(const void* data, GLsizei size)
{
    bindVAO();
    ib.setData(data, size);
}

void LowLevelGeometry::setBuffersData(const void* vdata, GLsizei vsize,
                                      const void* idata, GLsizei isize)
{
    bindVAO();
    vb.setData(vdata, vsize);
    ib.setData(idata, isize);
}

void LowLevelGeometry::bindVAO() const { glBindVertexArray(vao); }

void LowLevelGeometry::freeResources() { glDeleteVertexArrays(1, &vao); }
