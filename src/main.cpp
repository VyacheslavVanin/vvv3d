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
          layer(new SpriteLayer())
    {
        addLayer( layer );
    }

    std::shared_ptr<SpriteLayer> getLayer() const {return layer;}

private:
    std::shared_ptr<SpriteLayer> layer;
};


int main(int argc, char** argv)
{
    SpriteEngine* engine = new SpriteEngine(argc,argv);
    auto s = std::make_shared<Sprite>("data/images/aaa.png");
    engine->getLayer()->add(s);

    engine->run();


    return 0;
}


