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

    const vvv::vector4f &getTexturePosition() const;
    GLuint getHeight() const {return height;}
    GLuint getWidth() const {return width;}
    void bind(GLuint unit) const { tex->bind(unit); }
    void bind() const {tex->bind(); }

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
    TextureManager();

    std::shared_ptr<Texture> get(const std::string& name) const;

    void add(std::shared_ptr<LowLevelTexture> texture, const std::string& name);
    void add(const std::string& filename, const std::string& name);
    void add(const std::string& filename);
    bool contain(const std::string& name);
    void remove(const std::string& name);
    void clear( );

    std::vector<std::string> listNames()const;

private:
    mutable std::unordered_map<std::string, std::shared_ptr<Texture>>         texs;
    mutable std::unordered_map<std::string, std::shared_ptr<LowLevelTexture>> lowLevelTexs;
};

#endif // TEXTURE_H
