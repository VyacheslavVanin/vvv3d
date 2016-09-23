#include "sprite.h"
#include <core/engine.h>


Sprite::Sprite(const std::string& name)
{
    setTexture(name);
    transform.setScale(texture->getWidth(), texture->getHeight(), 1);
}

void Sprite::setTexture(std::shared_ptr<Texture> tex)
{
    texture = tex;
}

void Sprite::setTexture(const std::string& name)
{
    const auto& e = Engine::getActiveEngine();
    const auto& tm = e->getTextureManager();
    texture = tm.get(name);
}

const Texture& Sprite::getTexture() const
{
    return *texture;
}
