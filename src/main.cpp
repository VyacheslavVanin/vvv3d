#include <memory>
#include <iomanip>
#include <string>
#include <core/layeredengine.h>
#include <std/spritelayer.h>

class SpriteEngine : public LayeredEngine
{
public:
    SpriteEngine(int argc, char** argv, const char* windowName="SpriteEngine")
        : LayeredEngine(argc,argv, windowName),
          layer(new SpriteLayer()),
          s()
    {
        addLayer( layer );
        s = std::make_shared<Sprite>("data/images/aaa.png");
        layer->add(s);
    }

protected:
    void onDraw()
    {
        LayeredEngine::onDraw();
        s->transform.rotate(0.01, 0, 0, 1);
    }

private:
    std::shared_ptr<SpriteLayer> layer;
    std::shared_ptr<Sprite> s;

    // Engine interface
};


int main(int argc, char** argv)
{
    SpriteEngine* engine = new SpriteEngine(argc,argv);
    engine->run();

    return 0;
}


