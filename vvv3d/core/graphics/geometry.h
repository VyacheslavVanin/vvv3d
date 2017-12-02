#pragma once
#include "lowlevel/lowlevelgeometry.h"
#include <memory>
#include <unordered_map>

namespace vvv3d {

class Geometry {
public:
    Geometry(const void* vertexData, GLsizei dataSize, const GLuint* indices,
             GLsizei numIndices, const VertexAttributes& attributes,
             GLenum mode);
    Geometry(const VertexAttributes& attributes, GLenum mode);
    ~Geometry();

    void updateVB(void* data, GLsizei size);
    void updateIB(void* data, GLsizei size);
    void updateBuffers(void* vdata, GLsizei vsize, void* idata, GLsizei isize);

private:
    std::unique_ptr<LowLevelGeometry> geometry;
    GLenum mode;
    GLsizei numIndices;
    // Geometry interface
public:
    void draw() const;
};

class GeometryManager {
public:
    GeometryManager();
    void add(const std::string& name, std::unique_ptr<Geometry> geom);
    const Geometry& get(const std::string& name);
    std::vector<std::string> listNames() const;
    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Geometry>> geometries;
};
}

