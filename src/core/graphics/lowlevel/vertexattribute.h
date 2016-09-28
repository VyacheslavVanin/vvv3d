#ifndef VERTEXATTRIBUTE_H
#define VERTEXATTRIBUTE_H
#include "openglprovider.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <functional>
#include "utils/myutils.h"

enum class ATTRIB_LOCATION : GLuint
{
    POSITION,
    NORMAL,
    COLOR,
    TEXCOORD
};

const char *getAttribLocationName(ATTRIB_LOCATION attrib);

void bindAttribLocations(GLuint program);
GLsizei     sizeOfComponent(GLenum componentType);


class VertexAttribDesc
{
public:
    VertexAttribDesc( ATTRIB_LOCATION location, GLint numComponents,
                      GLenum componentType)
        : location(location), numComponents(numComponents),
          componentType(componentType)
    {}

    ATTRIB_LOCATION getLocation()       const {return location;}
    GLint           getNumComponents()  const {return numComponents;}
    GLenum          getComponentType()  const {return componentType;}
    GLsizei         size()              const {return numComponents*
                                               sizeOfComponent(componentType);}

private:
    ATTRIB_LOCATION location;
    GLint           numComponents;
    GLenum          componentType;
};

class VertexAttributes
{
private:

    struct VertexAttribute
    {
        VertexAttribute(GLuint location, GLint numComponents,
                        GLenum componentType, size_t offset, GLsizei stride)
                    :   location(location), numComponents(numComponents),
                        componentType(componentType),
                        offset((const void*)offset), stride(stride)
        {}

        GLuint      location;
        GLint       numComponents;
        GLenum      componentType;
        const void* offset;
        GLsizei     stride;
    };

public:
    VertexAttributes(const std::initializer_list<VertexAttribDesc>& attribs);
    void enable()const;
    void disable()const;

private:
    std::vector<VertexAttribute> attributes;
    GLsizei     stride;
};


#endif // VERTEXATTRIBUTE_H
