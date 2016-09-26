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

    void setTexture(std::shared_ptr<Texture> tex);
    const Texture& getTexture() const;

private:
    std::shared_ptr<Texture> texture;
};

#endif // SPRITE_H
