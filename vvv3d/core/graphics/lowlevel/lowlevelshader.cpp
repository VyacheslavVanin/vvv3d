#include "lowlevelshader.h"
#include "utils/myutils.h"
#include <core/graphics/lowlevel/openglprovider.h>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <string>

using namespace vvv3d;

static GLint ShaderStatus(GLuint shader, GLenum param)
{
    GLint status, length;
    glGetShaderiv(shader, param, &status);
    if (status != GL_TRUE) {
        GLchar buffer[1024];
        glGetShaderInfoLog(shader, 1024, &length, buffer);
        std::cout << "Shader: " << buffer << std::endl;
    }
    return status;
}

static GLuint createFromString(const char* source, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const int sourceSize = static_cast<int>(strlen(source));
    glShaderSource(shader, 1, &source, &sourceSize);
    glCompileShader(shader);
    if (ShaderStatus(shader, GL_COMPILE_STATUS) != GL_TRUE) {
        throw std::logic_error("Failed to compile shader ... ");
    }
    return shader;
}

LowLevelShader::LowLevelShader(const char* filename, GLenum shaderType)
    : shader()
{
    try {
        using namespace std;
        const string source = loadFileToString(filename);
        shader = ::createFromString(source.c_str(), shaderType);
    }
    catch (std::ios_base::failure& f) {
        std::cerr << "Failed to read shader from file " << filename << "\n";
        throw;
    }
    catch (std::exception& e) {
        std::cerr << "Failed to create shader from source in " << filename
                  << "\n";
        throw;
    }
}

LowLevelShader::LowLevelShader(LowLevelShader&& other) noexcept
    : shader(other.shader)
{
    other.shader = 0;
}

LowLevelShader& LowLevelShader::operator=(LowLevelShader&& other) noexcept
{
    glDeleteShader(shader);
    shader = other.shader;
    other.shader = 0;
    return *this;
}

LowLevelShader::~LowLevelShader() { glDeleteShader(shader); }

LowLevelShader LowLevelShader::fromString(const char* source, GLenum shaderType)
{
    LowLevelShader ret;
    ret.shader = ::createFromString(source, shaderType);
    return ret;
}

LowLevelShader::LowLevelShader() : shader(0) {}
