#include "lowlevelshaderprogram.hpp"
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <functional>
#include <stdexcept>
#include <vvv3d/std/log.hpp>

using namespace vvv3d;

LowLevelShaderProgram::~LowLevelShaderProgram() { glDeleteProgram(program); }

static GLint ProgramStatus(GLuint program, GLenum param)
{
    GLint status, length;
    glGetProgramiv(program, param, &status);
    if (status != GL_TRUE) {
        GLchar buffer[1024];
        glGetProgramInfoLog(program, 1024, &length, buffer);

        std::stringstream ss;
        ss << "Program: " << buffer;
        LOG(ss.str());
    }
    return status;
}

LowLevelShaderProgram::LowLevelShaderProgram() : program(~0u) {}

LowLevelShaderProgram::LowLevelShaderProgram(
    GLuint vertShader, GLuint fragShader, GLuint geomShader,
    const std::function<void(GLuint)>& vertexAttributesBinder)
    : program(~0u)
{
    CreateProgram(vertShader, fragShader, geomShader, vertexAttributesBinder);
}

LowLevelShaderProgram::LowLevelShaderProgram(
    LowLevelShaderProgram&& other) noexcept
    : program(other.program)
{
    other.program = 0;
}

LowLevelShaderProgram& LowLevelShaderProgram::
operator=(LowLevelShaderProgram&& other) noexcept
{
    glDeleteProgram(program);
    program = other.program;
    other.program = 0;
    return *this;
}

void LowLevelShaderProgram::activate() const { glUseProgram(program); }

void LowLevelShaderProgram::CreateProgram(
    GLuint vertshader, GLuint fragshader, GLuint geomshader,
    const std::function<void(GLuint)>& attributeBindings)
{
    program = glCreateProgram();
    glAttachShader(program, vertshader);
    glAttachShader(program, fragshader);
    if (geomshader)
        glAttachShader(program, geomshader);

    attributeBindings(program);

    glLinkProgram(program);
    if (ProgramStatus(program, GL_LINK_STATUS) != GL_TRUE) {
        throw std::logic_error("Failed to link program");
    }
    glUseProgram(program);
    glValidateProgram(program);
    if (ProgramStatus(program, GL_VALIDATE_STATUS) != GL_TRUE) {
        throw std::logic_error("Program validation failed");
    }
}
