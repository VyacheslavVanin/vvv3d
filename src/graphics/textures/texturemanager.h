#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "texture.h"
#include <memory>
#include <string>
#include <unordered_map>

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

#endif // TEXTUREMANAGER_H
