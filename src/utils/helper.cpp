#include "helper.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <boost/gil/gil_all.hpp>
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/extension/io/png_io.hpp>

LowLevelTexture* readFromPng(const char* filename)
{
    using namespace boost::gil;
    image<rgba8_pixel_t> im;

    png_read_image(filename, im);

    im._view = flipped_up_down_view(im._view);

    const size_t size = im._view.size();
    const size_t numChannels = im._view.num_channels();
    const size_t width = im.dimensions().x;
    const size_t height = im.dimensions().y;

    std::vector<uint8_t> data(size*numChannels);
    uint32_t c = 0;

    for(auto p:im._view)
        for(size_t i = 0; i < numChannels; ++i)
            data[c++] = p[i];

    return new LowLevelTexture(data.data(),width,height,GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
}

struct myvertex
{
    union
    {
        struct{
        float x,    y,  z;
        float nx,  ny, nz;
        float tx,  ty;};
        float arr[8];
    };
    bool operator==(const myvertex& o)
    {
        for(int i = 0; i < 8; ++i)
            if( arr[i] != o.arr[i]) return false;
        return true;
    }
};




template<typename T>
T getFromStream(std::istream& str)
{
    T ret;
    str >> ret;
    return ret;
}

std::istream& operator>>(std::istream& str, myvertex& vertex)
{
    for(size_t i = 0; i < 8; ++i)
        str >> vertex.arr[i];
    return str;
}

template<typename F>
void doNTimes(size_t N, const F& f )
{
    for(size_t i = 0; i < N; ++i)
        f();
}

Geometry* loadGeometryFrom_vvv3d(const char* filename)
{
    using namespace std;
    ifstream f;
    f.exceptions( ifstream::failbit | ifstream::badbit );
    f.open(filename);

    vector<myvertex> vertices;
    vector<GLuint>   indices;
    const size_t num_vertices = getFromStream<size_t>(f);

    doNTimes( num_vertices, [&vertices,&f]()
                            {vertices.push_back( getFromStream<myvertex>(f));});

    const size_t num_indices= getFromStream<size_t>(f);
    doNTimes( num_indices, [&indices,&f]()
                            {indices.push_back( getFromStream<GLuint>(f) );});


    auto ret = new Geometry( vertices.data(), vertices.size()*sizeof(myvertex),
                                   indices.data(),  indices.size(),
                               VertexAttributes({
                                     VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
                                     VertexAttribDesc(ATTRIB_LOCATION::NORMAL,   3, GL_FLOAT),
                                     VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT),
                                                }),
                               GL_TRIANGLES);
    return ret;
}


#if defined ENABLE_3DS
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>

static void assembleBuffers( Lib3dsMesh* mesh,
                      std::vector<myvertex>& vertices,
                      std::vector<GLuint>& indices)
{
    const size_t numFaces = mesh->faces;

    Lib3dsVector* normals = new Lib3dsVector[numFaces*3];
    lib3ds_mesh_calculate_normals(mesh,normals);

    for( size_t f = 0; f < numFaces; ++f)
        for( size_t v = 0; v < 3; ++v){
            const size_t cindex = mesh->faceL[f].points[v];
            const Lib3dsPoint&  p = mesh->pointL[ cindex ];
            const Lib3dsVector& n = normals[3*f + v];
            const Lib3dsTexel&  t = mesh->texelL[cindex];
            const myvertex cvertex = {
                                        p.pos[0], p.pos[1], p.pos[2],
                                        n[0], n[1], n[2],
                                        t[0], t[1],
                                     };
            const auto vbegin   = vertices.begin();
            const auto vend     = vertices.end();
            const auto pos = find(vbegin, vend, cvertex);
            if(pos != vend ){ // allready in vertices
                const GLuint i = distance(vbegin, pos); // index of element in vertices
                indices.push_back(i);}
            else{ // not in vertices yet
                const GLuint i = vertices.size();
                indices.push_back(i);
                vertices.push_back(cvertex);}}

    delete[] normals;
}

SimpleGeometry* loadGeometryFrom3ds(const char* filename)
{
    Lib3dsFile* model = lib3ds_file_load(filename);
    Lib3dsMesh* meshes = model->meshes;
    std::vector<myvertex> vertices;
    std::vector<GLuint>   indices;
    assembleBuffers(meshes, vertices, indices );

    auto ret = new SimpleGeometry( vertices.data(), vertices.size()*sizeof(myvertex),
                                   indices.data(),  indices.size(),
                               VertexAttributes({
                                     VertexAttribDesc(ATTRIB_LOCATION::POSITION, 3, GL_FLOAT),
                                     VertexAttribDesc(ATTRIB_LOCATION::NORMAL,   3, GL_FLOAT),
                                     VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT),
                                                }),
                               GL_TRIANGLES);
    return ret;
}
#endif


