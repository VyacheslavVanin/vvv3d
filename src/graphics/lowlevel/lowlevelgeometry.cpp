#include "lowlevelgeometry.h"



LowLevelGeometry::LowLevelGeometry(const std::shared_ptr<LowLevelBuffer> &vb, const std::shared_ptr<LowLevelBuffer> &ib, const VertexAttributes &attrib) : vb(vb), ib(ib), vao(~0u)
{
    glGenVertexArrays(1, &vao);
    bindVAO();
    vb->bind();
    ib->bind();
    attrib.enable();
}

LowLevelGeometry::~LowLevelGeometry() {freeResources();}

void LowLevelGeometry::Draw(GLenum mode, GLsizei count) const
{
    static GLuint activeVAO = ~0l;
    if( activeVAO == vao )
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    else {  activeVAO = vao;
        bindVAO();
        ib->bind(); // WORKAROUND: needed due to bug in some drivers
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);}
}

void LowLevelGeometry::setVertexBufferData(const void *data, GLsizei size)
{
    bindVAO();
    vb->setData(data,size);
}

void LowLevelGeometry::setIndexBufferData(const void *data, GLsizei size)
{
    bindVAO();
    ib->setData(data,size);
}

void LowLevelGeometry::setBuffersData(const void *vdata, GLsizei vsize, const void *idata, GLsizei isize)
{
    bindVAO();
    vb->setData(vdata,vsize);
    ib->setData(idata,isize);
}

void LowLevelGeometry::bindVAO() const {glBindVertexArray(vao);}

void LowLevelGeometry::freeResources()
{
    glDeleteVertexArrays(1,&vao);
}
