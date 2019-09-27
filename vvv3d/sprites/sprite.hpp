#pragma once
#include <memory>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

class Sprite {
public:
    Sprite();
    Sprite(const Sprite&) = default;
    Sprite& operator=(const Sprite&) = default;
    ~Sprite() = default;

    void setTexture(const vvv3d::Texture& texture) { this->texture = &texture; }
    const vvv3d::Texture& getTexture() const;

    vvv3d::Transform transform;

private:
    const vvv3d::Texture* texture;
};

void drawSprite(const vvv3d::Camera& camera, const Sprite& spr);

template <typename A, template <typename, typename> class C>
inline void drawSprites(const vvv3d::Camera& camera, const C<Sprite, A>& sprs)
{
    auto& shaderman = getShaderManager();
    auto& geomman = getGeometryManager();
    auto& sh = shaderman.get("sprite");
    const auto& g = geomman.get("sprite");

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
} // namespace vvv3d
