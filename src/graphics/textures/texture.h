#ifndef TEXTURE_H
#define TEXTURE_H
#include <memory>
#include <string>
#include <unordered_map>
#include <graphics/lowlevel/lowleveltexture.h>
#include "vvvmath/linalg.h"
#include "utils/helper.h"

class Texture
{
public:
    ~Texture()  {}

    const vvv::vector4f& getTexturePosition() const {return texturePosition;}
    GLuint getHeight() const {return height;}
    GLuint getWidth() const {return width;}
    void bind(GLuint unit) { tex->bind(unit); }
    void bind() {tex->bind(); }

    Texture(const std::shared_ptr<LowLevelTexture>& tex,
            const vvv::vector4f& texturePosition=vvv::vector4f(0,0,1,1))
        : tex(tex), texturePosition(texturePosition),
          width(tex->getWidth() * texturePosition.z),
          height( tex->getHeight()* texturePosition.w)
    {}

    GLint getFormat() const {return tex->getFormat();}
    GLuint getID() const { return tex->getID(); }

private:
    std::shared_ptr<LowLevelTexture> tex;// pointer to real texture
    vvv::vector4f texturePosition;//store position in real texture (x,y)-offset, (z,w)-width,height
    GLuint width;
    GLuint height;
};


class TextureManager
{
public:
    TextureManager() : texs(), lowLevelTexs() {}

    std::shared_ptr<Texture> get(const std::string& name)
    {
        try{
            return texs.at(name); }
        catch(...) {
            add(name);
            return texs.at(name); }
    }

    void add(std::shared_ptr<LowLevelTexture> texture, const std::string& name)
    {
        if( texs.count(name) > 0 ) return;
        lowLevelTexs[name] = texture;
        texs[name].reset(new Texture(texture));
    }

    void add(const std::string& filename, const std::string& name)
    {
        if( texs.count(name) > 0 ) return;
        std::shared_ptr<LowLevelTexture>  im(readFromPng(filename.c_str()));
        lowLevelTexs[filename] = im;
        texs[name].reset(new Texture(im));
    }

    void add(const std::string& filename) { add(filename, filename); }

    bool contain(const std::string& name) { return texs.find(name)!=texs.end(); }

    void remove(const std::string& name)
    {
        auto i = texs.find(name);
        if(i!=texs.end())
            texs.erase(i);
    }

    void clear( )
    {
        texs.clear();
        lowLevelTexs.clear();
    }

    std::vector<std::string> listNames()const
    {
        std::vector<std::string> ret;
        for(auto kv: texs)
            ret.push_back(kv.first);
        return ret;
    }

private:
    mutable std::unordered_map<std::string, std::shared_ptr<Texture>>         texs;
    mutable std::unordered_map<std::string, std::shared_ptr<LowLevelTexture>> lowLevelTexs;
};

#endif // TEXTURE_H
