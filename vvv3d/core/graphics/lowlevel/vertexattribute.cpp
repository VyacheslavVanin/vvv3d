#include "vertexattribute.h"

namespace vvv3d {

static const std::map<ATTRIB_LOCATION,std::string> attributesNamesBindings =
{
    {ATTRIB_LOCATION::POSITION, "va_position"  },
    {ATTRIB_LOCATION::TEXCOORD, "va_texCoord"  },
    {ATTRIB_LOCATION::NORMAL,   "va_normal"    },
    {ATTRIB_LOCATION::COLOR,    "va_color"     },
};

const char *getAttribLocationName(ATTRIB_LOCATION attrib)
{
    auto i = attributesNamesBindings.find( attrib );
    if( i == attributesNamesBindings.end() )
        throw std::logic_error("No such attrib");
    return (*i).second.c_str();
}

void bindAttribLocations(GLuint program)
{
    for(const auto& a: attributesNamesBindings)
        glBindAttribLocation(program, static_cast<GLuint>(a.first),
                             a.second.c_str());
}

GLsizei sizeOfComponent(GLenum componentType)
{
    static const std::map<GLenum,GLsizei> typeSizeMapping =
    {
        {GL_FLOAT,          sizeof(GLfloat )},
#ifdef GL_DOUBLE
        {GL_DOUBLE,         sizeof(GLdouble)},
#endif
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

VertexAttributes::VertexAttributes(const std::initializer_list<VertexAttribDesc>& attribs)
    : attributes(),stride(0)
{
    // Calculate vertex size
    for(const auto& d: attribs)
        stride += d.size();

    size_t offset = 0;
    for(const auto& d: attribs){
        attributes.push_back(VertexAttribute(
                                 static_cast<GLuint>( d.getLocation()),
                                 d.getNumComponents(),
                                 d.getComponentType(),
                                 offset,
                                 stride));
        offset += d.size();
    }
}

void VertexAttributes::enable() const
{
    for(const auto& a: attributes){
        glVertexAttribPointer(a.location, a.numComponents, a.componentType,
                              GL_FALSE, a.stride, a.offset);
        glEnableVertexAttribArray(a.location);
    }
}

}

