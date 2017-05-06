#include "textureatlas.h"
#include "texturepacker.h"
#include <utils/helper.h>

using namespace vvv3d;

using namespace std;

TextureAtlas::TextureAtlas(size_t width, size_t height,
                           const vector<string> &filenames,
                           const vector<string> &names, unsigned int border)
    : atlas(make_shared<LowLevelTexture>(nullptr, width, height,
                                         GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE)),
      textures(), width(width), height(height)
{
    std::vector<shared_ptr<LowLevelTexture>> lltexs;
    std::transform(filenames.begin(), filenames.end(),back_inserter(lltexs),
                   [](auto& name)
    {
        return shared_ptr<LowLevelTexture>(readFromPng(name.c_str()));
    });
    constructorFunction(width, height, names, lltexs, border);
}

TextureAtlas::TextureAtlas(size_t width, size_t height,
                           const std::initializer_list<string> &names,
                           unsigned int border)
    : atlas(make_shared<LowLevelTexture>(nullptr, width, height,
                                         GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE)),
      textures(), width(width), height(height)
{
    std::vector<shared_ptr<LowLevelTexture>> lltexs;
    std::transform(names.begin(), names.end(), back_inserter(lltexs),
                   [](auto& name)
    {
        return shared_ptr<LowLevelTexture>(readFromPng(name.c_str()));
    });
    constructorFunction(width, height, names, lltexs, border);
}

static bool textureCompareHeightFirst(const std::shared_ptr<Texture>& l,
                                      const std::shared_ptr<Texture>& r)
{
    return l->getHeight() < r->getHeight() ? true :
           l->getHeight() > r->getHeight() ? false:
           l->getWidth() < r->getWidth();
}

static pair<GLuint, GLuint> textureGetSize(const std::shared_ptr<Texture>& t)
{
    return make_pair(t->getWidth(), t->getHeight());
}

void TextureAtlas::constructorFunction(size_t width, size_t height,
                           const vector<string> &names,
                           const vector<shared_ptr<LowLevelTexture>> &texsList,
                           unsigned int border)
{
    vector<shared_ptr<Texture>> texsToPack;
    for(const auto& t: texsList)
        texsToPack.push_back(make_shared<Texture>(t));


    for(size_t i = 0; i < texsList.size(); ++i)
        textures[ names[i] ] = texsToPack[i];

    auto setTextureOffset = [&](std::shared_ptr<Texture>& t, int32_t xoff, int32_t yoff,
                                int32_t border)
    {
        t->texturePosition.x = float(xoff + border) / width;
        t->texturePosition.y = float(yoff + border) / height;
    };

    vector<shared_ptr<Texture>> notPlaced;
    texsToPack = pack2d(texsToPack, static_cast<ssize_t>(width),
                        static_cast<ssize_t>(height), textureCompareHeightFirst,
                        textureGetSize, setTextureOffset, notPlaced, border);
    if(notPlaced.size() > 0)
        throw std::logic_error("Failed to fill texture atlas."
                               " Atlas too small for images.");

    const size_t MAX_BYTES = width*height*4;
    auto buff = make_unique<GLubyte[]>(MAX_BYTES);
    for(size_t i = 0; i < MAX_BYTES; ++i)
        buff.get()[i] = 0;

    // Clear atlass
    atlas->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0,0, width, height,
                    GL_RGBA, GL_UNSIGNED_BYTE, buff.get() );

    // Draw all images on atlass
    for(auto& t: texsToPack){
        t->bind();
        glReadPixels(0, 0, t->getWidth(), t->getHeight(),
                     GL_RGBA, GL_UNSIGNED_BYTE, buff.get());
        atlas->bind();

        const GLuint texOffsetX = t->getTexturePosition().x * width;
        const GLuint texOffsetY = t->getTexturePosition().y * height;
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        texOffsetX, texOffsetY,
                        t->width, t->height,
                        GL_RGBA, GL_UNSIGNED_BYTE, buff.get());
        t->tex = atlas;

        const float widthUnit = 1.0f / width;
        const float heightUnit = 1.0f / height;
        t->texturePosition.z = widthUnit * (t->width - 1);
        t->texturePosition.w = heightUnit * (t->height - 1);
        t->texturePosition.x += 0.5*widthUnit;
        t->texturePosition.y += 0.5*heightUnit;
    }
}

TextureAtlas::TextureAtlas(size_t width, size_t height,
                           const vector<shared_ptr<LowLevelTexture>> &texsList,
                           const vector<string> &names, unsigned int border)
    : atlas(make_shared<LowLevelTexture>(nullptr, width, height,
                                         GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE)),
      textures(), width(width), height(height)
{
    constructorFunction(width, height, names, texsList, border);
}


std::shared_ptr<Texture> TextureAtlas::get(const string &name) const
{
    return textures.at(name);
}

std::vector<string> TextureAtlas::listNames() const
{
    std::vector<string> ret;
    for(const auto& i: textures)
        ret.push_back(i.first);
    return ret;
}
