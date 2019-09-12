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
    COUNT, ///< count ATTRIB_LOCATION enums, Not for binding locations
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
    explicit VertexAttributes(const std::vector<VertexAttribDesc>& attribs);
    void enable() const;
    void disable() const;

    GLsizei getStride() const { return stride; }

    /// Return offset for specified attrib location
    /// If location empty return -1
    GLsizei getOffset(ATTRIB_LOCATION location) const
    {
        return offsets.at(static_cast<size_t>(location));
    }
    /// Return size in bytes for specified attrib location
    /// If location empty return -1
    GLsizei getAttribSize(ATTRIB_LOCATION location) const
    {
        return sizes.at(static_cast<size_t>(location));
    }

private:
    std::vector<VertexAttribute> attributes;
    std::vector<GLsizei> offsets;
    std::vector<GLsizei> sizes;
    GLsizei stride;
};
} // namespace vvv3d
