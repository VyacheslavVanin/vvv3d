#include "vertexattribute.hpp"
#include "vvvstlhelper.hpp"

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>

#include <vvv3d/utils/myutils.hpp>

namespace vvv3d {

static const std::unordered_map<ATTRIB_LOCATION, std::string>
    attributesNamesBindings = {
        {ATTRIB_LOCATION::POSITION, "va_position"},
        {ATTRIB_LOCATION::TEXCOORD, "va_texCoord"},
        {ATTRIB_LOCATION::NORMAL, "va_normal"},
        {ATTRIB_LOCATION::COLOR, "va_color"},
        {ATTRIB_LOCATION::TANGENT, "va_tangent"},
        {ATTRIB_LOCATION::BITANGENT, "va_bitangent"},
};

const char* getAttribLocationName(ATTRIB_LOCATION attrib)
{
    auto i = attributesNamesBindings.find(attrib);
    if (i == attributesNamesBindings.end())
        throw std::logic_error("No such attrib");
    return (*i).second.c_str();
}

void bindAttribLocations(GLuint program)
{
    for (const auto& a : attributesNamesBindings)
        glBindAttribLocation(program, static_cast<GLuint>(a.first),
                             a.second.c_str());
}

GLsizei sizeOfComponent(GLenum componentType)
{
    static const std::unordered_map<GLenum, GLsizei> typeSizeMapping = {
        {GL_FLOAT, sizeof(GLfloat)},
        {GL_BYTE, sizeof(GLbyte)},
        {GL_SHORT, sizeof(GLshort)},
        {GL_INT, sizeof(GLint)},
        {GL_UNSIGNED_BYTE, sizeof(GLubyte)},
        {GL_UNSIGNED_SHORT, sizeof(GLushort)},
        {GL_UNSIGNED_INT, sizeof(GLuint)},
#ifdef GL_DOUBLE
        {GL_DOUBLE, sizeof(GLdouble)},
#endif
    };
    auto iter = typeSizeMapping.find(componentType);
    if (iter == typeSizeMapping.end())
        throw std::logic_error("No such type");
    return (*iter).second;
}

VertexAttributes::VertexAttributes(const std::vector<VertexAttribDesc>& attribs)
    : attributes(), offsets(static_cast<size_t>(ATTRIB_LOCATION::COUNT)),
      stride(0)
{
    // Calculate vertex size
    for (const auto& d : attribs)
        stride += d.size();

    size_t offset = 0;
    for (const auto& d : attribs) {
        offsets[static_cast<size_t>(d.getLocation())] = offset;
        attributes.push_back(VertexAttribute(
            static_cast<GLuint>(d.getLocation()), d.getNumComponents(),
            d.getComponentType(), d.getNormalized(), offset, stride));
        offset += d.size();
    }
}
VertexAttributes::VertexAttributes(
    const std::initializer_list<VertexAttribDesc>& attribs)
    : VertexAttributes(std::vector<VertexAttribDesc>(attribs))
{
}

void VertexAttributes::enable() const
{
    static const auto isNeedAttribI = [](const VertexAttribute& a) {
        const static std::initializer_list<GLenum> glIntTypes = {
            GL_UNSIGNED_BYTE,  GL_BYTE, GL_SHORT,
            GL_UNSIGNED_SHORT, GL_INT,  GL_UNSIGNED_INT};
        return !a.normalized && contain(glIntTypes, a.componentType);
    };

    for (const auto& a : attributes) {
        if (isNeedAttribI(a)) {
            glVertexAttribIPointer(a.location, a.numComponents, a.componentType,
                                   a.stride, a.offset);
        }
        else {
            glVertexAttribPointer(a.location, a.numComponents, a.componentType,
                                  a.normalized, a.stride, a.offset);
        }
        glEnableVertexAttribArray(a.location);
    }
}
} // namespace vvv3d
