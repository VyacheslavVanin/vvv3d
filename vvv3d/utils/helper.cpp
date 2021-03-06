#include "helper.hpp"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <vector>
#include <vvv3d/utils/log.hpp>

namespace vvv3d {

struct myvertex {
    union {
        struct {
            float x, y, z;
            float nx, ny, nz;
            float tx, ty;
        };
        float arr[8];
    };
    bool operator==(const myvertex& o)
    {
        for (int i = 0; i < 8; ++i)
            if (arr[i] != o.arr[i])
                return false;
        return true;
    }
};

template <typename T> T getFromStream(std::istream& str)
{
    T ret;
    str >> ret;
    return ret;
}

std::istream& operator>>(std::istream& str, myvertex& vertex)
{
    for (size_t i = 0; i < 8; ++i)
        str >> vertex.arr[i];
    return str;
}

template <typename F> void doNTimes(size_t N, const F& f)
{
    for (size_t i = 0; i < N; ++i)
        f();
}

Geometry* loadGeometryFrom_vvv3d(const char* filename)
{
    using namespace std;
    ifstream f;
    f.exceptions(ifstream::failbit | ifstream::badbit);
    f.open(filename);

    vector<myvertex> vertices;
    vector<GLuint> indices;
    const size_t num_vertices = getFromStream<size_t>(f);

    auto readVertex = [&vertices, &f]() {
        vertices.push_back(getFromStream<myvertex>(f));
    };
    doNTimes(num_vertices, readVertex);

    const size_t num_indices = getFromStream<size_t>(f);
    auto readIndex = [&indices, &f]() {
        indices.push_back(getFromStream<GLuint>(f));
    };
    doNTimes(num_indices, readIndex);

    const auto vertices_size = vertices.size() * sizeof(myvertex);
    auto ret = new Geometry(
        vertices.data(), vertices_size, indices.data(), indices.size(),
        VertexAttributes({
            VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
            VertexAttribDesc(ATTRIB_LOCATION::NORMAL, 3, GL_FLOAT),
            VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT),
        }));
    return ret;
}

thread_local int bench::depth = 0;
int bench::enable = 1;

bench::bench(const std::string& msg)
    : msg(msg), start(std::chrono::system_clock::now())
{
    ++depth;
}

bench::~bench()
{
    --depth;
    if (!enable)
        return;
    const auto now = std::chrono::system_clock::now();
    const auto us =
        std::chrono::duration_cast<std::chrono::microseconds>(now - start);

    std::stringstream ss;
    ss << std::string(2 * depth, ' ') << msg << ": " << us.count() << " us";
    LOG(ss.str());
}

#if defined ENABLE_3DS
#include <lib3ds/file.hpp>
#include <lib3ds/material.hpp>
#include <lib3ds/mesh.hpp>

static void assembleBuffers(Lib3dsMesh* mesh, std::vector<myvertex>& vertices,
                            std::vector<GLuint>& indices)
{
    const size_t numFaces = mesh->faces;

    Lib3dsVector* normals = new Lib3dsVector[numFaces * 3];
    lib3ds_mesh_calculate_normals(mesh, normals);

    for (size_t f = 0; f < numFaces; ++f)
        for (size_t v = 0; v < 3; ++v) {
            const size_t cindex = mesh->faceL[f].points[v];
            const Lib3dsPoint& p = mesh->pointL[cindex];
            const Lib3dsVector& n = normals[3 * f + v];
            const Lib3dsTexel& t = mesh->texelL[cindex];
            const myvertex cvertex = {
                p.pos[0], p.pos[1], p.pos[2], n[0], n[1], n[2], t[0], t[1],
            };
            const auto vbegin = vertices.begin();
            const auto vend = vertices.end();
            const auto pos = find(vbegin, vend, cvertex);
            if (pos != vend) { // allready in vertices
                // index of element in vertices
                const GLuint i = distance(vbegin, pos);
                indices.push_back(i);
            }
            else { // not in vertices yet
                const GLuint i = vertices.size();
                indices.push_back(i);
                vertices.push_back(cvertex);
            }
        }

    delete[] normals;
}

SimpleGeometry* loadGeometryFrom3ds(const char* filename)
{
    Lib3dsFile* model = lib3ds_file_load(filename);
    Lib3dsMesh* meshes = model->meshes;
    std::vector<myvertex> vertices;
    std::vector<GLuint> indices;
    assembleBuffers(meshes, vertices, indices);

    auto ret = new SimpleGeometry(
        vertices.data(), vertices.size() * sizeof(myvertex), indices.data(),
        indices.size(),
        VertexAttributes({
            VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
            VertexAttribDesc(ATTRIB_LOCATION::NORMAL, 3, GL_FLOAT),
            VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT),
        }));
    return ret;
}
#endif
} // namespace vvv3d
