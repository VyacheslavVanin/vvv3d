#include "texturemanager.h"
#include "utils/helper.h"

using namespace vvv3d;

TextureManager::TextureManager() : texs() {}

Texture& TextureManager::get(const std::string& name) const
{
    try {
        return *texs.at(name).get();
    }
    catch (...) {
        const_cast<TextureManager*>(this)->add(name);
        return *texs.at(name).get();
    }
}

void TextureManager::add(LowLevelTexture* texture,
                         const std::string& name)
{
    if (texs.count(name) > 0)
        return;
    texs[name].reset(new Texture(texture));
}

void TextureManager::add(const std::string& filename, const std::string& name)
{
    if (texs.count(name) > 0)
        return;
    std::shared_ptr<LowLevelTexture> im(readFromPng(filename.c_str()));
    texs[name].reset(new Texture(im));
}

void TextureManager::add(const std::string& filename)
{
    add(filename, filename);
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
