#include "textline.h"
#include <vvv3d/vvvmath/linalg.h>
#include <vvv3d/core/engine.h>
#if __GNUC__ > 4
#include <codecvt>
#else
#include <boost/locale/encoding_utf.hpp>
#endif

struct textVertex
{
    textVertex() : pos(), texcoord() {}
    vvv::vector2f pos;
    vvv::vector2f texcoord;
};
struct GlyphQuad
{
    GlyphQuad() : bottomLeft(), topRight(), topLeft(), bottomRight() {}
    textVertex bottomLeft;
    textVertex topRight;
    textVertex topLeft;
    textVertex bottomRight;
};

std::shared_ptr<Geometry>
createTextGeometry(const Font& font, const std::u32string& text)
{
    auto ret = std::make_shared<Geometry>(
               VertexAttributes({
                   VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
                   VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT)}),
               GL_TRIANGLES);
    updateTextGeometry(ret, font, text);
    return ret;
}

void updateTextGeometry(const std::shared_ptr<Geometry>& in,
                        const Font& font, const std::u32string& text )
{
    const size_t numChars = text.size();
    std::vector<GlyphQuad> vertices(numChars);
    std::vector<GLsizei> indices(numChars*6);

    float advance = 0;
    size_t iVerts = 0;
    size_t iInds  = 0;
    const auto& texture = font.getTexture();
    for(auto c: text)
    {
        const auto& g = font.getGlyph(c);
        GlyphQuad& currentQuad = vertices[iVerts];
        const GLsizei nVerts = static_cast<GLsizei>(iVerts)*4;
        const float x       = advance + g.xoffset;
        const float rightx  = x + g.width;
        const float y       = g.yoffset;
        const float top_y   = y + g.height;
        const float tmultiplyer = 1.0f / texture.getWidth();
        const float left_tx = g.textureOffsetX*tmultiplyer;
        const float bottom_ty = g.textureOffsetY*tmultiplyer;
        const float right_tx = left_tx + g.width*tmultiplyer;
        const float top_ty   = bottom_ty + g.height*tmultiplyer;

        currentQuad.bottomLeft.pos.set(x, y);
        currentQuad.bottomLeft.texcoord.set(left_tx, bottom_ty);

        currentQuad.bottomRight.pos.set(rightx, y);
        currentQuad.bottomRight.texcoord.set(right_tx, bottom_ty);

        currentQuad.topLeft.pos.set(x, top_y);
        currentQuad.topLeft.texcoord.set(left_tx, top_ty);

        currentQuad.topRight.pos.set(rightx, top_y);
        currentQuad.topRight.texcoord.set(right_tx, top_ty);

        // TODO: indices same for all strings but deiffers in size,
        // so it is possible to keep one index array for all strings
        // and store only number of indices used.
        indices[iInds + 0] = nVerts + 0;
        indices[iInds + 1] = nVerts + 1;
        indices[iInds + 2] = nVerts + 2;
        indices[iInds + 3] = nVerts + 0;
        indices[iInds + 4] = nVerts + 1;
        indices[iInds + 5] = nVerts + 3;
        advance += g.advance;
        ++iVerts;
        iInds += 6;
    }

    in->updateBuffers( vertices.data(),
                       static_cast<GLsizei>(vertices.size()*sizeof(GlyphQuad)),
                       indices.data(),
                       static_cast<GLsizei>(indices.size()*sizeof(GLsizei)));
}

std::u32string toU32(const std::string& u8)
{
#if __GNUC__ > 4
    std::locale::global(std::locale("en_US.utf8"));
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv32;
    return conv32.from_bytes(u8);
#else
    return boost::locale::conv::utf_to_utf<char32_t>(u8.c_str(),
                                                     u8.c_str() + u8.size());
#endif
}


std::shared_ptr<Geometry>
createTextGeometry(const Font& f, const std::string& str)
{
    return createTextGeometry(f, toU32(str));
}

void updateTextGeometry(const std::shared_ptr<Geometry>& in, const Font& font,
                        const std::string& text)
{
    updateTextGeometry(in, font, toU32(text));
}
