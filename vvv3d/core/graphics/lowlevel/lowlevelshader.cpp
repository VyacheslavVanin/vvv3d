#include <core/graphics/lowlevel/openglprovider.h>
#include <memory>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdint>
#include <stdexcept>
#include "lowlevelshader.h"
#include "utils/myutils.h"

using namespace vvv3d;

static GLint ShaderStatus( GLuint shader, GLenum param)
{
    GLint status, length;
    glGetShaderiv( shader, param, &status);
    if( status != GL_TRUE ){
        GLchar buffer[1024];
        glGetShaderInfoLog( shader, 1024, &length, buffer);
        std::cout << "Shader: " << buffer << std::endl;}
    return status;
}

static GLuint createFromString(const char* source, GLenum shaderType)
{
    GLuint shader = glCreateShader( shaderType );
    const int sourceSize  = static_cast<int>(strlen(source));
    glShaderSource(  shader, 1, &source, &sourceSize);
    glCompileShader( shader );
    if( ShaderStatus( shader, GL_COMPILE_STATUS) != GL_TRUE ) {
        throw std::logic_error("Failed to compile shader ... ");}
    return shader;
}

LowLevelShader::LowLevelShader(const char *filename, GLenum shaderType) : shader()
{
    using namespace std;
    const string source = loadFileToString( filename );

    shader = ::createFromString(source.c_str(), shaderType);
}

LowLevelShader::LowLevelShader(LowLevelShader &&other) noexcept
    : shader(other.shader)
{
    other.shader = 0;
}

LowLevelShader &LowLevelShader::operator=(LowLevelShader &&other) noexcept
{
    glDeleteShader(shader);
    shader = other.shader;
    other.shader = 0;
    return *this;
}

LowLevelShader::~LowLevelShader()
{
    glDeleteShader( shader );
}

LowLevelShader LowLevelShader::fromString(const char *source, GLenum shaderType)
{
    LowLevelShader ret;
    ret.shader = ::createFromString(source, shaderType);
    return ret;
}

LowLevelShader LowLevelShader::fragmentFromString(const char* source)
{
    return fromString(source, GL_FRAGMENT_SHADER);
}

LowLevelShader LowLevelShader::vertexFromString(const char* source)
{
    return fromString(source, GL_VERTEX_SHADER);
}

LowLevelShader::LowLevelShader() : shader(0) {}
