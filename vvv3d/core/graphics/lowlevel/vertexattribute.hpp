#pragma once
#include "openglprovider.hpp"
#include <initializer_list>
#include <vector>

namespace vvv3d {

enum class ATTRIB_LOCATION : GLuint {
    POSITION,
    NORMAL,
    COLOR,
    TEXCOORD,
    TANGENT,
    BITANGENT,
};

const char* getAttribLocationName(ATTRIB_LOCATION attrib);

void bindAttribLocations(GLuint program);
GLsizei sizeOfComponent(GLenum componentType);

class VertexAttribDesc {
public:
    VertexAttribDesc(ATTRIB_LOCATION location, GLint numComponents,
                     GLenum componentType, bool normalized = false)
        : location(location), numComponents(numComponents),
          componentType(componentType), normalized(normalized)
    {
    }

    ATTRIB_LOCATION getLocation() const { return location; }
    GLint getNumComponents() const { return numComponents; }
    GLenum getComponentType() const { return componentType; }
    GLsizei size() const
    {
        return numComponents * sizeOfComponent(componentType);
    }

    bool getNormalized() const { return normalized; }

private:
    ATTRIB_LOCATION location;
    GLint numComponents;
    GLenum componentType;
    bool normalized;
};

class VertexAttributes {
private:
    struct VertexAttribute {
        VertexAttribute(GLuint location, GLint numComponents,
                        GLenum componentType, bool normalized, size_t offset,
                        GLsizei stride)
            : location(location), numComponents(numComponents),
              componentType(componentType), normalized(normalized),
              offset((const void*)offset), stride(stride)
        {
        }

        GLuint location;
        GLint numComponents;
        GLenum componentType;
        GLboolean normalized;
        const void* offset;
        GLsizei stride;
    };

public:
    explicit VertexAttributes(
        const std::initializer_list<VertexAttribDesc>& attribs);
    void enable() const;
    void disable() const;

private:
    std::vector<VertexAttribute> attributes;
    GLsizei stride;
};
} // namespace vvv3d
