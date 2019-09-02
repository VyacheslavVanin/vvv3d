#include "lowlevelshader.hpp"

#include "utils/myutils.hpp"
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <vvv3d/std/log.hpp>

#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

using namespace vvv3d;
using vvv::helper::format;

const std::string& shader_type_to_string(GLenum shader_type)
{
    static const std::string vertex = "vertex";
    static const std::string fragment = "fragment";
    static const std::string geometry = "geometry";
    switch (shader_type) {
    case GL_VERTEX_SHADER: return vertex;
    case GL_FRAGMENT_SHADER: return fragment;
    case GL_GEOMETRY_SHADER: return geometry;
    default: throw std::logic_error("Invalid shader type");
    }
}

static GLint ShaderStatus(GLuint shader, GLenum param)
{
    GLint status, length;
    glGetShaderiv(shader, param, &status);
    if (status != GL_TRUE) {
        GLchar buffer[1024];
        glGetShaderInfoLog(shader, 1024, &length, buffer);
        LOG(format("Shader: @", buffer));
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
        throw std::logic_error(format("Failed to compile @ shader ... :\n@",
                                      shader_type_to_string(shaderType),
                                      source));
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
        LOG(format("Failed to read @ shader from file @",
                   shader_type_to_string(shaderType), filename));
        throw;
    }
    catch (std::exception& e) {
        LOG(format("Failed to create @ shader from source in @",
                   shader_type_to_string(shaderType), filename));
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
