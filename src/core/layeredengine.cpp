#include "layeredengine.h"

LayeredEngine::LayeredEngine(int argc, char **argv, const char *windowName) :
    Engine(argc,argv,windowName){}

void LayeredEngine::initialSetup()
{
    for(auto l: layers)
        l->initialSetup();
}

void LayeredEngine::onDraw()
{
    for(auto l: layers)
        l->onDraw();
}

void LayeredEngine::onResize(int x, int y)
{
    for(auto l: layers)
        l->onResize(x,y);
}

void LayeredEngine::addLayer(std::shared_ptr<ILayer> layer)
{
    layers.push_back(layer);
}
