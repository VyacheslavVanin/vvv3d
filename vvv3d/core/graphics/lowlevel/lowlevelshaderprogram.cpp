#include "lowlevelshaderprogram.h"
#include <core/graphics/lowlevel/openglprovider.h>
#include <functional>
#include <iostream>
#include <stdexcept>

using namespace vvv3d;

LowLevelShaderProgram::~LowLevelShaderProgram() { glDeleteProgram(program); }

static GLint ProgramStatus(GLuint program, GLenum param)
{
    GLint status, length;
    glGetProgramiv(program, param, &status);
    if (status != GL_TRUE) {
        GLchar buffer[1024];
        glGetProgramInfoLog(program, 1024, &length, buffer);
        std::cout << "Program: " << buffer << std::endl;
    }
    return status;
}

LowLevelShaderProgram::LowLevelShaderProgram() : program(~0u) {}

LowLevelShaderProgram::LowLevelShaderProgram(
    GLuint vshader, GLuint fshader,
    const std::function<void(GLuint)>& vertexAttributesBinder)
    : program(~0u)
{
    CreateProgram(vshader, fshader, vertexAttributesBinder);
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
    program       = other.program;
    other.program = 0;
    return *this;
}

void LowLevelShaderProgram::activate() const
{
    static GLuint currentProgram = ~0u;
    if (currentProgram != program) {
        glUseProgram(program);
        currentProgram = program;
    }
}

void LowLevelShaderProgram::CreateProgram(
    GLuint vshader, GLuint fshader,
    const std::function<void(GLuint)>& attributeBindings)
{
    program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);

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
