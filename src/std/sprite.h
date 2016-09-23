#ifndef SPRITE_H
#define SPRITE_H
#include <memory>
#include <core/transform.h>
#include <graphics/textures/texture.h>

class Sprite
{
public:
    Transform transform;

    Sprite() = default;
    Sprite(const std::string& name);

    void setTexture(std::shared_ptr<Texture> tex);

    /**
     * @brief setTexture set texture by name in TextureManager
     * @param name name in Texture manager */
    void setTexture(const std::string& name);

    const Texture& getTexture() const;

private:
    std::shared_ptr<Texture> texture;
};

#endif // SPRITE_H
