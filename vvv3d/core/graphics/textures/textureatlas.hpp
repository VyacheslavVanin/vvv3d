#pragma once
#include "texture.hpp"
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vvv3d {

class TextureAtlas {
public:
    static std::vector<std::unique_ptr<TextureAtlas>>
    pack(size_t width, size_t height,
         const std::vector<LowLevelTexture*>& texsList,
         const std::vector<std::string>& names, unsigned int border = 0);

    static std::vector<std::unique_ptr<TextureAtlas>>
    pack(size_t width, size_t height, const std::vector<std::string>& filenames,
         const std::vector<std::string>& names, unsigned int border = 0);

    static std::vector<std::unique_ptr<TextureAtlas>>
    pack(size_t width, size_t height, const std::vector<std::string>& filenames,
         unsigned int border = 0);

    static std::unique_ptr<TextureAtlas>
    makeAtlas(std::shared_ptr<LowLevelTexture>&& atlas,
              const std::vector<vvv::vector4f>& coords,
              const std::vector<std::string>& names);
    static std::unique_ptr<TextureAtlas>
    makeAtlas(const std::string& texture_file_name,
              const std::vector<vvv::vector4f>& coords,
              const std::vector<std::string>& names);

    const Texture& get(const std::string& name) const;
    std::vector<std::string> listNames() const;

    auto getInternal() { return atlas; }

private:
    TextureAtlas(size_t width, size_t height);
    friend class TextureManager;
    std::shared_ptr<LowLevelTexture> atlas;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    size_t width;
    size_t height;
};

} // namespace vvv3d
