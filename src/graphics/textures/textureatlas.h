#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include "texture.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <initializer_list>
#include <vector>

class TextureAtlas
{
public:
    TextureAtlas(size_t width, size_t height,
                 const std::vector<std::string>& filenames,
                 const std::vector<std::string>& names);
    TextureAtlas(size_t width, size_t height,
                 const std::vector<std::shared_ptr<LowLevelTexture>>& texsList,
                 const std::vector<std::string>& names);
    TextureAtlas(size_t width, size_t height,
                 const std::initializer_list<std::string>& names);

    std::shared_ptr<Texture> get(const std::string& name)const;
    std::vector<std::string> listNames()const;

    auto getInternal() {return atlas;}

private:
    std::shared_ptr<LowLevelTexture> atlas;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    size_t width;
    size_t height;
    void constructorFunction(size_t width, size_t height,
                 const std::vector<std::string> &names,
                 const std::vector<std::shared_ptr<LowLevelTexture>> &texsList);
};

#endif // TEXTUREATLAS_H
