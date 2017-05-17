#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "texture.h"
#include "textureatlas.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vvv3d {

class TextureManager {
public:
    TextureManager();

    Texture& get(const std::string& name) const;

    void add(LowLevelTexture* texture, const std::string& name);
    void add(const std::string& filename, const std::string& name);
    void add(const std::string& filename);

    void addAtlas(TextureAtlas&& atlas);
    bool contain(const std::string& name);
    void remove(const std::string& name);
    void clear();

    std::vector<std::string> listNames() const;

private:
    mutable std::unordered_map<std::string, std::unique_ptr<Texture>> texs;
};
}

#endif // TEXTUREMANAGER_H
