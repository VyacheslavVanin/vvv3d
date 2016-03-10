#include <graphics/lowlevel/openglprovider.h>
#include "lowlevelbuffer.h"
#include <stdexcept>

LowLevelBuffer::LowLevelBuffer(GLenum target, const void *data, size_t size)
    : target(target), buffer(~0u), size(0), sizemax(0)
{
    glGenBuffers(1, &buffer);
    setData(data, size);
}

LowLevelBuffer::~LowLevelBuffer()
{
    glDeleteBuffers(1,&buffer);
}

void LowLevelBuffer::bind() const
{
    if( buffer==(~0u) ) throw std::logic_error("Buffer not initialized");
    glBindBuffer(target, buffer);
}

void LowLevelBuffer::setData(const void *data, size_t size)
{
    bind();

    if(size > this->sizemax) {
        glBufferData(target, size, data, GL_STATIC_DRAW);
        sizemax = size; }
    else
        glBufferSubData(target, 0, size, data);
    this->size = size;
}

void LowLevelBuffer::updateData(size_t offset,
                                void *data, size_t size)
{
    const size_t upperBound = offset+size;
    if( upperBound  > this->size)
        throw std::out_of_range("LowLevelBuffer::updateData");

    bind();
    glBufferSubData(target, offset, size, data);
}

void LowLevelBuffer::reset()
{
    size = 0;
    sizemax = 0;
}

