#include <graphics/lowlevel/openglprovider.h>
#include "lowlevelshaderprogram.h"
#include <iostream>
#include <stdexcept>
#include <functional>


LowLevelShaderProgram::~LowLevelShaderProgram()
{
    glDeleteProgram(program);
}


static GLint ProgramStatus( GLuint program, GLenum param)
{
    GLint status, length;
    GLchar buffer[1024];
    glGetProgramiv( program, param, &status);
    if( status != GL_TRUE ){
        glGetProgramInfoLog( program, 1024, &length, buffer);
        std::cout << "Program: " << buffer << std::endl;}
    return status;
}

LowLevelShaderProgram::LowLevelShaderProgram():program(~0u){}

LowLevelShaderProgram::LowLevelShaderProgram(GLuint vshader, GLuint fshader,
                                             const std::function<void(GLuint)> &f) : program(~0u)
{
    CreateProgram(vshader,fshader, f );
}

void LowLevelShaderProgram::activate() const
{
    static GLuint currentProgram = ~0u;
    if( currentProgram != program ){
        glUseProgram( program );
        currentProgram = program;}
}

void LowLevelShaderProgram::CreateProgram(GLuint vshader, GLuint fshader, const std::function<void (GLuint)> &attributeBindings)
{
    program = glCreateProgram();
    glAttachShader( program, vshader );
    glAttachShader( program, fshader );

    attributeBindings(program);

    glLinkProgram( program );       if( ProgramStatus(program, GL_LINK_STATUS)      != GL_TRUE ) { throw std::logic_error("Failed to link program");}
    glUseProgram(program);
    glValidateProgram( program );   if( ProgramStatus(program, GL_VALIDATE_STATUS)  != GL_TRUE ) { throw std::logic_error("Program validation failed");}
}
