#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include "texture.h"
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vvv3d {

class TextureAtlas {
public:
    TextureAtlas(size_t width, size_t height,
                 const std::vector<std::string>& filenames,
                 const std::vector<std::string>& names,
                 unsigned int border = 0);
    TextureAtlas(size_t width, size_t height,
                 const std::vector<LowLevelTexture*>& texsList,
                 const std::vector<std::string>& names,
                 unsigned int border = 0);
    TextureAtlas(size_t width, size_t height,
                 const std::initializer_list<std::string>& names,
                 unsigned int border = 0);

    const Texture& get(const std::string& name) const;
    std::vector<std::string> listNames() const;

    auto getInternal() { return atlas; }

private:
    friend class TextureManager;
    std::shared_ptr<LowLevelTexture> atlas;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    size_t width;
    size_t height;
    void constructorFunction(
        size_t width, size_t height, const std::vector<std::string>& names,
        const std::vector<LowLevelTexture*>& texsList,
        unsigned int border = 0);
};
}

#endif // TEXTUREATLAS_H
