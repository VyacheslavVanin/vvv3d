#pragma once

#define QT_OPENGL_ES_3_2
#include <QOpenGLExtraFunctions>

extern QOpenGLExtraFunctions* __vvv3d_qt_opengl_funcs;

#define glActiveTexture __vvv3d_qt_opengl_funcs->glActiveTexture
#define glAttachShader __vvv3d_qt_opengl_funcs->glAttachShader
#define glBindAttribLocation __vvv3d_qt_opengl_funcs->glBindAttribLocation
#define glBindBuffer __vvv3d_qt_opengl_funcs->glBindBuffer
#define glBindFramebuffer __vvv3d_qt_opengl_funcs->glBindFramebuffer
#define glBindTexture __vvv3d_qt_opengl_funcs->glBindTexture
#define glBindVertexArray __vvv3d_qt_opengl_funcs->glBindVertexArray
#define glBlendFunc __vvv3d_qt_opengl_funcs->glBlendFunc
#define glBufferData __vvv3d_qt_opengl_funcs->glBufferData
#define glBufferSubData __vvv3d_qt_opengl_funcs->glBufferSubData
#define glCheckFramebufferStatus                                               \
    __vvv3d_qt_opengl_funcs->glCheckFramebufferStatus
#define glClear __vvv3d_qt_opengl_funcs->glClear
#define glClearColor __vvv3d_qt_opengl_funcs->glClearColor
#define glCompileShader __vvv3d_qt_opengl_funcs->glCompileShader
#define glCreateProgram __vvv3d_qt_opengl_funcs->glCreateProgram
#define glCreateShader __vvv3d_qt_opengl_funcs->glCreateShader
#define glDebugMessageCallback __vvv3d_qt_opengl_funcs->glDebugMessageCallback
#define glDeleteBuffers __vvv3d_qt_opengl_funcs->glDeleteBuffers
#define glDeleteFramebuffers __vvv3d_qt_opengl_funcs->glDeleteFramebuffers
#define glDeleteProgram __vvv3d_qt_opengl_funcs->glDeleteProgram
#define glDeleteShader __vvv3d_qt_opengl_funcs->glDeleteShader
#define glDeleteTextures __vvv3d_qt_opengl_funcs->glDeleteTextures
#define glDeleteVertexArrays __vvv3d_qt_opengl_funcs->glDeleteVertexArrays
#define glDisable __vvv3d_qt_opengl_funcs->glDisable
#define glDrawBuffers __vvv3d_qt_opengl_funcs->glDrawBuffers
#define glDrawElements __vvv3d_qt_opengl_funcs->glDrawElements
#define glEnable __vvv3d_qt_opengl_funcs->glEnable
#define glEnableVertexAttribArray                                              \
    __vvv3d_qt_opengl_funcs->glEnableVertexAttribArray
#define glFramebufferTexture2D __vvv3d_qt_opengl_funcs->glFramebufferTexture2D
#define glGenBuffers __vvv3d_qt_opengl_funcs->glGenBuffers
#define glGenFramebuffers __vvv3d_qt_opengl_funcs->glGenFramebuffers
#define glGenTextures __vvv3d_qt_opengl_funcs->glGenTextures
#define glGenVertexArrays __vvv3d_qt_opengl_funcs->glGenVertexArrays
#define glGetAttribLocation __vvv3d_qt_opengl_funcs->glGetAttribLocation
#define glGetBooleanv __vvv3d_qt_opengl_funcs->glGetBooleanv
#define glGetIntegerv __vvv3d_qt_opengl_funcs->glGetIntegerv
#define glGetProgramInfoLog __vvv3d_qt_opengl_funcs->glGetProgramInfoLog
#define glGetProgramiv __vvv3d_qt_opengl_funcs->glGetProgramiv
#define glGetShaderInfoLog __vvv3d_qt_opengl_funcs->glGetShaderInfoLog
#define glGetShaderiv __vvv3d_qt_opengl_funcs->glGetShaderiv
#define glGetString __vvv3d_qt_opengl_funcs->glGetString
#define glGetTexImage __vvv3d_qt_opengl_funcs->glGetTexImage
#define glGetUniformLocation __vvv3d_qt_opengl_funcs->glGetUniformLocation
#define glLinkProgram __vvv3d_qt_opengl_funcs->glLinkProgram
#define glReadPixels __vvv3d_qt_opengl_funcs->glReadPixels
#define glScissor __vvv3d_qt_opengl_funcs->glScissor
#define glShaderSource __vvv3d_qt_opengl_funcs->glShaderSource
#define glTexImage2D __vvv3d_qt_opengl_funcs->glTexImage2D
#define glTexParameteri __vvv3d_qt_opengl_funcs->glTexParameteri
#define glTexSubImage2D __vvv3d_qt_opengl_funcs->glTexSubImage2D
#define glUniform1f __vvv3d_qt_opengl_funcs->glUniform1f
#define glUniform1fv __vvv3d_qt_opengl_funcs->glUniform1fv
#define glUniform1i __vvv3d_qt_opengl_funcs->glUniform1i
#define glUniform1iv __vvv3d_qt_opengl_funcs->glUniform1iv
#define glUniform2f __vvv3d_qt_opengl_funcs->glUniform2f
#define glUniform3f __vvv3d_qt_opengl_funcs->glUniform3f
#define glUniform3fv __vvv3d_qt_opengl_funcs->glUniform3fv
#define glUniform4f __vvv3d_qt_opengl_funcs->glUniform4f
#define glUniform4fv __vvv3d_qt_opengl_funcs->glUniform4fv
#define glUniformMatrix3fv __vvv3d_qt_opengl_funcs->glUniformMatrix3fv
#define glUniformMatrix4fv __vvv3d_qt_opengl_funcs->glUniformMatrix4fv
#define glUseProgram __vvv3d_qt_opengl_funcs->glUseProgram
#define glValidateProgram __vvv3d_qt_opengl_funcs->glValidateProgram
#define glVertexAttribIPointer __vvv3d_qt_opengl_funcs->glVertexAttribIPointer
#define glVertexAttribPointer __vvv3d_qt_opengl_funcs->glVertexAttribPointer
#define glViewport __vvv3d_qt_opengl_funcs->glViewport
