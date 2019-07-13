#include "geometry.hpp"
#include "lowlevel/lowlevelbuffer.hpp"
#include "lowlevel/vertexattribute.hpp"
#include <memory>
#include <vvv3d/std/log.hpp>

using namespace std;
using namespace vvv3d;
using vvv::helper::format;

Geometry::Geometry(const void* vertexData, GLsizei dataSize,
                   const GLuint* indices, GLsizei numIndices,
                   const VertexAttributes& attributes, GLenum mode)
    : geometry(new LowLevelGeometry(
          LowLevelBuffer(GL_ARRAY_BUFFER, vertexData, dataSize),
          LowLevelBuffer(GL_ELEMENT_ARRAY_BUFFER, indices,
                         numIndices * sizeof(GLuint)),
          attributes)),
      mode(mode), numIndices(numIndices)
{
}

Geometry::Geometry(const VertexAttributes& attributes, GLenum mode)
    : geometry(new LowLevelGeometry(
          LowLevelBuffer(GL_ARRAY_BUFFER, 0, 0),
          LowLevelBuffer(GL_ELEMENT_ARRAY_BUFFER, 0, 0), attributes)),
      mode(mode), numIndices(0)
{
}

Geometry::~Geometry() {}

void Geometry::updateVB(void* data, GLsizei size)
{
    geometry->setVertexBufferData(data, size);
}

void Geometry::updateIB(void* data, GLsizei size)
{
    geometry->setIndexBufferData(data, size);
    numIndices = size / sizeof(GLint);
}

void Geometry::updateBuffers(void* vdata, GLsizei vsize, void* idata,
                             GLsizei isize)
{
    geometry->setBuffersData(vdata, vsize, idata, isize);
    numIndices = isize / sizeof(GLint);
}

void Geometry::draw() const { geometry->Draw(mode, numIndices); }

GeometryManager::GeometryManager() : geometries() {}

void GeometryManager::add(const string& name, std::unique_ptr<Geometry> geom)
{
    if (contain(name))
        return;

    geometries[name].swap(geom);
}

void GeometryManager::add(const std::string& name,
                          const std::function<std::unique_ptr<Geometry>()>& f)
{
    initializers[name] = f;
}

const Geometry& GeometryManager::get(const string& name) const
try {
    return *geometries.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(format("failed to get geometry \"@\"", name));
    throw;
}

const Geometry& GeometryManager::get(const string& name)
try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end())
            add(name, it->second());
    }
    return *geometries.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(format("failed to get geometry \"@\"", name));
    throw;
}

bool GeometryManager::contain(const std::string& name) const
{
    return geometries.find(name) != geometries.end();
}

void GeometryManager::clear() { geometries.clear(); }

std::vector<string> GeometryManager::listNames() const
{
    std::vector<std::string> ret;
    for (const auto& kv : geometries)
        ret.push_back(kv.first);
    return ret;
}
