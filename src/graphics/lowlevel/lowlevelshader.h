#ifndef LOWLEVELSHADER_H
#define LOWLEVELSHADER_H
#include "openglprovider.h"

class LowLevelShader
{
public:
    LowLevelShader(const char* filename, GLenum shaderType);
    LowLevelShader(const LowLevelShader&) = default;
    LowLevelShader(LowLevelShader&&) = default;
    ~LowLevelShader();

    GLuint get() const {return shader;}

    static LowLevelShader&& fromString(const char* source, GLenum shaderType);
    static LowLevelShader&& fragmentFromString(const char* source);
    static LowLevelShader&& vertexFromString(const char* source);

private:
    LowLevelShader();
    GLuint shader;
};

#endif // LOWLEVELSHADER_H
