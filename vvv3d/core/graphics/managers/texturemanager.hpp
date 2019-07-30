#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <vvv3d/core/graphics/textures/texture.hpp>
#include <vvv3d/core/graphics/textures/textureatlas.hpp>

#include <vvvcfg/vvvcfg.hpp>

namespace vvv3d {

class TextureManager {
public:
    TextureManager();

    /** @brief Get texture by name. If not found throw std::out_of_range */
    Texture& get(const std::string& name) const;

    /** @brief Get texture by name. If not found try to load texture as if name
     *  is uri */
    Texture& get(const std::string& name);
    Texture& getDefault() const;

    const TextureShared& getShared(const std::string& name) const;
    const TextureShared& getShared(const std::string& name);
    const TextureShared& getDefaultShared() const;

    void add(LowLevelTexture* texture, const std::string& name);
    void add(const std::string& filename, const std::string& name);

    /** @brief Same as add() but overwrite existing texture with new */
    void addForce(LowLevelTexture* texture, const std::string& name);

    /** @brief Add texture. Throws on fail
     * @param filename */
    void add(const std::string& filename);

    /** @brief Add texture. Try load from file. Fail silently.
     * @param filename */
    void safe_add(const std::string& filename);

    void addAtlas(TextureAtlas&& atlas);
    void addAtlas(std::unique_ptr<TextureAtlas> atlas);
    void addAtlas(std::vector<std::unique_ptr<TextureAtlas>>&& atlas);
    bool contain(const std::string& name) const;
    void remove(const std::string& name);
    void clear();

    std::vector<std::string> listNames() const;

    // Config have to contain something like this:
    // textures
    //    images
    //        named_list={a: "images/image1.png", b: "images/image2.png"}
    //        list=["images/image1.png", "images/image2.png"]
    //        a="images/image1.png"
    //        b="images/image2.png"
    //    atlases
    //        a1_list  size=512 border=0 images=["images/image1.png",
    //                                           "images/image2.png"]
    //        a1_named size=512 border=0 images={a: "images/image1.png",
    //                                           b: "images/image2.png"}
    //        a2_packed texture="atlas_texture.png" \
    //                  images={image_1: [0,0, 16, 16],
    //                          image_2: [16, 0, 16, 42]}
    void load(const vvv::CfgNode& cfg);
    void load(const std::string& cfg);
    void load(std::istream& cfg);

private:
    std::unordered_map<std::string, TextureShared> texs;
};
} // namespace vvv3d