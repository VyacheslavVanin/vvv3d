#ifndef LAYEREDENGINE_H
#define LAYEREDENGINE_H
#include <vector>
#include <memory>
#include <core/engine.h>
#include <core/ILayer.h>

class LayeredEngine : public Engine
{
public:
    LayeredEngine(int argc, char** argv, const char* windowName = "vvv3d");
    ~LayeredEngine() {}
    // Engine interface
protected:
    void initialSetup();
    void onDraw();
    void onResize(int x, int y);

    /**
     * @brief addLayer Add new layer to the Engine.
     *  Use this at constructor or somewhere before calling Engine.run() as
     *  initialSetup() called inside Engine.run().
     * @param layer */
    void addLayer(std::shared_ptr<ILayer> layer);

private:
    std::vector<std::shared_ptr<ILayer>> layers;
};

#endif // LAYEREDENGINE_H
