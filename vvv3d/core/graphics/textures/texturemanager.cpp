#include "texturemanager.hpp"
#include "utils/helper.hpp"

using namespace vvv3d;

TextureManager::TextureManager() : texs() {
    add(vvv3d::makeDummyTexture(256, 256, 32), "default");
}

Texture& TextureManager::get(const std::string& name)
{
    auto i = texs.find(name);
    if (i != texs.end())
        return *i->second;

    add(name);
    const auto* cthis = this;
    return cthis->get(name);
}

Texture& TextureManager::get(const std::string& name) const
{
    return *texs.at(name).get();
}

void TextureManager::add(LowLevelTexture* texture, const std::string& name)
{
    if (contain(name))
        return;
    texs[name].reset(new Texture(texture));
}

void TextureManager::add(const std::string& filename, const std::string& name)
{
    if (contain(name))
        return;
    std::shared_ptr<LowLevelTexture> im(makeLLTexture(filename));
    texs[name].reset(new Texture(im));
}

void TextureManager::add(const std::string& filename)
{
    add(filename, filename);
}

void TextureManager::addAtlas(std::unique_ptr<TextureAtlas> atlas)
{
    addAtlas(std::move(*atlas));
}

void TextureManager::addAtlas(
    std::vector<std::unique_ptr<TextureAtlas>>&& atlases)
{
    for (auto& a : atlases)
        addAtlas(std::move(a));
}

void TextureManager::addAtlas(TextureAtlas&& atlas)
{
    for (auto i = atlas.textures.begin(); i != atlas.textures.end();) {
        texs.insert(std::move(*i));
        i = atlas.textures.erase(i);
    }
}

bool TextureManager::contain(const std::string& name)
{
    return texs.find(name) != texs.end();
}

void TextureManager::remove(const std::string& name)
{
    auto i = texs.find(name);
    if (i != texs.end())
        texs.erase(i);
}

void TextureManager::clear() { texs.clear(); }

std::vector<std::string> TextureManager::listNames() const
{
    std::vector<std::string> ret;
    for (auto& kv : texs)
        ret.push_back(kv.first);
    return ret;
}
