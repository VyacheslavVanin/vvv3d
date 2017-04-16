#ifndef SPRITE_H
#define SPRITE_H
#include <memory>
#include <vvv3d/vvv3d.h>

class Sprite
{
public:
    Sprite();
    Sprite(const Sprite&) = default;
    Sprite& operator=(const Sprite&) = default;
    ~Sprite() = default;

    void setTexture(Texture* texture) {this->texture = texture;}
    Texture* getTexture() const {return texture;}

    Transform transform;
private:
    Texture* texture;
};

void drawSprite(Engine& engine, const Camera& camera, const Sprite& spr);

template<typename A, template<typename, typename> class C >
inline void drawSprites(Engine& engine, const Camera& camera,
                        const C<Sprite, A>& sprs)
{
    auto& resman = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman = resman.getGeometryManager();
    auto sh = shaderman.get("sprite");
    auto g = geomman.get("sprite");

    sh->activate();
    sh->setViewProjection(camera.getViewProjection());
    for(const auto& s : sprs){
        const auto texture = s.getTexture();
        sh->setModel(s.transform.getModelMatrix());
        sh->setTexturePosition(texture->getTexturePosition());
        sh->setTexture0(*texture);
        g->draw();
    }
}


#endif
