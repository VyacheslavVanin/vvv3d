#pragma once
#include "texture.hpp"
#include "textureatlas.hpp"
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
    void addAtlas(std::unique_ptr<TextureAtlas> atlas);
    void addAtlas(std::vector<std::unique_ptr<TextureAtlas>>&& atlas);
    bool contain(const std::string& name);
    void remove(const std::string& name);
    void clear();

    std::vector<std::string> listNames() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> texs;
};
}

