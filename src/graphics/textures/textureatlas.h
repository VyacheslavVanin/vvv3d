#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include "texture.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <initializer_list>

class TextureAtlas
{
public:
    TextureAtlas(size_t width, size_t height,
                 const std::vector<std::string>& filenames,
                 const std::vector<std::string>& names);
    TextureAtlas(size_t width, size_t height,
                 const std::vector<std::shared_ptr<LowLevelTexture>>& textures,
                 const std::vector<std::string>& names);
    TextureAtlas(size_t width, size_t height,
                 const std::initializer_list<std::string>& names);

    std::shared_ptr<Texture> get(const std::string& name)const;
    std::vector<std::string> listNames()const;

private:
    std::shared_ptr<LowLevelTexture> atlas;
    std::unordered_map<std::string, std::shared_ptr<Texture>> texture;
    size_t width;
    size_t height;
};

#endif // TEXTUREATLAS_H
