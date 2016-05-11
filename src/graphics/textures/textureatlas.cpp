#include "textureatlas.h"
#include "texturepacker.h"

using namespace std;

TextureAtlas::TextureAtlas(size_t width, size_t height,
                           const vector<string> &filenames,
                           const vector<string> &names)
{

}

TextureAtlas::TextureAtlas(size_t width, size_t height,
                           const vector<shared_ptr<LowLevelTexture>> &textures,
                           const vector<string> &names)
{
}

TextureAtlas::TextureAtlas(size_t width, size_t height, const std::initializer_list<string> &names)
{

}

std::shared_ptr<Texture> TextureAtlas::get(const string &name) const
{

}

std::vector<string> TextureAtlas::listNames() const
{

}
