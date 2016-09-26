#include "sprite.h"
#include <core/engine.h>


void Sprite::setTexture(std::shared_ptr<Texture> tex)
{
    texture = tex;
}

const Texture& Sprite::getTexture() const
{
    return *texture;
}
