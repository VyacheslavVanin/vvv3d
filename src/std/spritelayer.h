#ifndef SPRITELAYER_H
#define SPRITELAYER_H
#include <vector>
#include <core/ILayer.h>
#include <graphics/camera.h>
#include <std/sprite.h>
class Engine;

class SpriteLayer : public ILayer
{
public:
    SpriteLayer(Engine* engine);

    // ILayer interface
public:
    void initialSetup();
    void onDraw();
    void onResize(int x, int y);

public:
    void clear() { sprites.clear(); }
    void add(std::shared_ptr<Sprite> s) { sprites.push_back(s);}
    Camera& getCamera() {return camera;}

private:
    Camera camera;
    std::vector<std::shared_ptr<Sprite>> sprites;
    Engine*              engine;
};

#endif // SPRITELAYER_H
