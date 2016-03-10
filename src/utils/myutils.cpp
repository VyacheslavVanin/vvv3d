#include "myutils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <map>

std::string loadFileToString(const char *filename)
{
    std::string ret;
    loadFileToString(filename,ret);
    return ret;
}

void loadFileToString(const char *filename, std::string &out)
{
    using namespace std;
    ifstream f;
    f.exceptions( ifstream::failbit | ifstream::badbit);
    f.open(filename);

    f.seekg(0, std::ios::end);
    out.reserve( f.tellg() );
    f.seekg(0, std::ios::beg);
    out.assign(istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

GLsizei sizeOfComponent(GLenum componentType)
{
    static const std::map<GLenum,GLsizei> typeSizeMapping =
    {
        {GL_FLOAT,          sizeof(GLfloat )},
        {GL_DOUBLE,         sizeof(GLdouble)},
        {GL_BYTE,           sizeof(GLbyte)  },
        {GL_SHORT,          sizeof(GLshort) },
        {GL_INT,            sizeof(GLint)   },
        {GL_UNSIGNED_BYTE,  sizeof(GLubyte) },
        {GL_UNSIGNED_SHORT, sizeof(GLushort)},
        {GL_UNSIGNED_INT,   sizeof(GLuint)  },
    };
    auto iter = typeSizeMapping.find(componentType);
    if( iter == typeSizeMapping.end() )
        throw std::logic_error("No such type");
    return (*iter).second;
}




