#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <vvv3d/core/input.h>

namespace vvv3d {

class ResourceManager;
class HAL;

class Engine {
public:
    Engine(int argc, char** argv, const char* windowName = "vvv3d");
    virtual ~Engine();

    ResourceManager& getResourceManager();
    const Input& getInput() const;

    void run();
    float getCurrentFps() const;
    int getViewportWidth() const;
    int getViewportHeight() const;
    static Engine& getActiveEngine();
    void setVSync(bool vsync);

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static Engine* activeEngine;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    int viewportWidth;
    int viewportHeight;
    std::unique_ptr<HAL> hal;
    std::unique_ptr<ResourceManager> resourceManager;
    friend class Input;
    Input input;
};
}

#endif // ENGINE
