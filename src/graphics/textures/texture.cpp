#include "texture.h"



TextureManager::TextureManager() : texs(), lowLevelTexs() {}

std::shared_ptr<Texture> TextureManager::get(const std::string &name) const
{
    try{
        return texs.at(name); }
    catch(...) {
        const_cast<TextureManager*>(this)->add(name);
        return texs.at(name); }
}

void TextureManager::add(std::shared_ptr<LowLevelTexture> texture, const std::string &name)
{
    if( texs.count(name) > 0 ) return;
    lowLevelTexs[name] = texture;
    texs[name].reset(new Texture(texture));
}

void TextureManager::add(const std::string &filename, const std::string &name)
{
    if( texs.count(name) > 0 ) return;
    std::shared_ptr<LowLevelTexture>  im(readFromPng(filename.c_str()));
    lowLevelTexs[filename] = im;
    texs[name].reset(new Texture(im));
}

void TextureManager::add(const std::string &filename) { add(filename, filename); }

bool TextureManager::contain(const std::string &name) { return texs.find(name)!=texs.end(); }

void TextureManager::remove(const std::string &name)
{
    auto i = texs.find(name);
    if(i!=texs.end())
        texs.erase(i);
}

void TextureManager::clear()
{
    texs.clear();
    lowLevelTexs.clear();
}

std::vector<std::string> TextureManager::listNames() const
{
    std::vector<std::string> ret;
    for(auto kv: texs)
        ret.push_back(kv.first);
    return ret;
}
