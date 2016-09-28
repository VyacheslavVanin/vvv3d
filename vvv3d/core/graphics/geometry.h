#ifndef SIMPLEGEOMETRY_H
#define SIMPLEGEOMETRY_H
#include <unordered_map>
#include "lowlevel/lowlevelgeometry.h"
#include <memory>

class Geometry
{
public:
    Geometry(const void*   vertexData, GLsizei dataSize,
                   const GLuint* indices,    GLsizei numIndices,
                   const VertexAttributes& attributes,
                   GLenum mode);
    Geometry(const VertexAttributes& attributes,
                   GLenum mode);
    ~Geometry();


    void updateVB(void* data, GLsizei size);
    void updateIB(void* data, GLsizei size);
    void updateBuffers(void* vdata, GLsizei vsize,
                       void* idata, GLsizei isize);

private:
    std::unique_ptr<LowLevelGeometry> geometry;
    GLenum                            mode;
    GLsizei                           numIndices;
    // Geometry interface
public:
    void draw() const;
};


class GeometryManager
{
public:
    void add(const std::string& name, std::shared_ptr<Geometry> geom);
    std::shared_ptr<Geometry> get(const std::string& name);
    std::vector<std::string> listNames()const;
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometries;
};


#endif // SIMPLEGEOMETRY_H
