#ifndef SPRITE_H
#define SPRITE_H
#include <memory>
#include <core/transform.h>
#include <graphics/textures/texture.h>
#include <core/engine.h>

class Sprite
{
public:
    Transform transform;

    void setTexture(std::shared_ptr<Texture> tex)
    {
        texture = tex;
    }

    /**
     * @brief setTexture set texture by name in TextureManager
     * @param name name in Texture manager */
    void setTexture(const std::string& name)
    {
        const auto& e = Engine::getActiveEngine();
        const auto& tm = e->getTextureManager();
        texture = tm.get(name);
    }

    const Texture& getTexture() const
    {
        return *texture;
    }

private:
    std::shared_ptr<Texture> texture;
};

#endif // SPRITE_H
