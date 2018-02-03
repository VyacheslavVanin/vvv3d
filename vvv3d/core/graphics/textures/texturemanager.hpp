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

    /** @brief Get texture by name. If not found throw std::out_of_range */
    Texture& get(const std::string& name) const;

    /** @brief Get texture by name. If not found try to load texture as if name
     *  is uri */
    Texture& get(const std::string& name);

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
} // namespace vvv3d
