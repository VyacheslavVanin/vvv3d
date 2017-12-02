#pragma once
#include "openglprovider.h"

namespace vvv3d {

class LowLevelShader {
public:
    LowLevelShader(const char* filename, GLenum shaderType);
    LowLevelShader(const LowLevelShader&) = delete;
    LowLevelShader& operator=(const LowLevelShader&) = delete;
    LowLevelShader(LowLevelShader&& other) noexcept;
    LowLevelShader& operator=(LowLevelShader&& other) noexcept;
    ~LowLevelShader();

    GLuint get() const { return shader; }

    static LowLevelShader fromString(const char* source, GLenum shaderType);

private:
    LowLevelShader();
    GLuint shader;
};
}

