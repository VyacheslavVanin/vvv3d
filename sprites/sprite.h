#ifndef SPRITE_H
#define SPRITE_H
#include <memory>
#include <vvv3d/vvv3d.h>

class Sprite {
public:
    Sprite();
    Sprite(const Sprite&) = default;
    Sprite& operator=(const Sprite&) = default;
    ~Sprite()                        = default;

    void setTexture(const vvv3d::Texture& texture) { this->texture = &texture; }
    const vvv3d::Texture& getTexture() const { return *texture; }

    vvv3d::Transform transform;

private:
    const vvv3d::Texture* texture;
};

void drawSprite(vvv3d::Engine& engine, const vvv3d::Camera& camera,
                const Sprite& spr);

template <typename A, template <typename, typename> class C>
inline void drawSprites(vvv3d::Engine& engine, const vvv3d::Camera& camera,
                        const C<Sprite, A>& sprs)
{
    auto& resman    = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman   = resman.getGeometryManager();
    auto& sh        = shaderman.get("sprite");
    const auto& g   = geomman.get("sprite");

    sh.activate();
    sh.setViewProjection(camera.getViewProjection());
    for (const auto& s : sprs) {
        const auto& texture = s.getTexture();
        sh.setModel(s.transform.getModelMatrix());
        sh.setTexturePosition(texture.getTexturePosition());
        sh.setTexture0(texture);
        g.draw();
    }
}

#endif
