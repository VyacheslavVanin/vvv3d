#ifndef LOWLEVELSHADERPROGRAM_H
#define LOWLEVELSHADERPROGRAM_H
#include "openglprovider.h"
#include <functional>
#include <vvvmath/linalg.h>

class LowLevelShaderProgram
{
public:
    LowLevelShaderProgram();
    LowLevelShaderProgram(GLuint vshader, GLuint fshader,
                          const std::function<void(GLuint)>& f=[](GLuint){} );
    ~LowLevelShaderProgram();

    void activate() const;

    GLint getAttributeLocation(const char* name) const {
        return glGetAttribLocation(  program, name);    }

    GLint  getUniformLocation(const char* name) const {
        return glGetUniformLocation( program, name);  }


    static void setUniform(GLint location, float d)
    {
        glUniform1f( location, d);
    }

    static void setUniform(GLint location, float* data, GLsizei num)
    {
        glUniform1fv(location, num, data );
    }

    static void setUniform(GLint location, int d)
    {
        glUniform1i( location, d);
    }

    static void setUniform(GLint location, int *data, GLsizei num)
    {
        glUniform1iv(location, num, data);
    }

    static void setUniform(GLint location, const vvv::vector2f& v)
    {
        glUniform2f(location, v.x, v.y);
    }

    static void setUniform(GLint location, const vvv::vector3f& v)
    {
        glUniform3f(location, v.x, v.y, v.z );
    }

    static void setUniform(GLint location, const vvv::vector4f& v)
    {
        glUniform4f(location, v.x, v.y, v.z, v.w );
    }

    static void setUniform(GLint location, const vvv::matrix33f& m)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE,
                           static_cast<const GLfloat*>(&m.matrix[0][0]));
    }

    static void setUniform(GLint location, const vvv::matrix44f &m)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE,
                           static_cast<const GLfloat*>(&m.matrix[0][0]));
    }

    void CreateProgram(GLuint vsh, GLuint fsh, const std::function<void(GLuint)>& f );
private:
    GLuint program;
};

#endif // LOWLEVELSHADERPROGRAM_H
