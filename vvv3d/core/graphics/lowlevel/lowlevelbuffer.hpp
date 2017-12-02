#pragma once
#include "openglprovider.hpp"

namespace vvv3d {

class LowLevelBuffer {
public:
    LowLevelBuffer(GLenum target, const void* data, size_t size);
    ~LowLevelBuffer();
    LowLevelBuffer(const LowLevelBuffer&) = delete;
    LowLevelBuffer& operator=(const LowLevelBuffer&) = delete;
    LowLevelBuffer(LowLevelBuffer&& other) noexcept;
    LowLevelBuffer& operator=(LowLevelBuffer&& other) noexcept;

    void bind() const;
    void setData(const void* data, size_t size);
    void updateData(size_t offset, void* data, size_t size);
    void reset();

private:
    GLenum target;
    GLuint buffer;
    size_t size;
    size_t sizemax;
};
}

