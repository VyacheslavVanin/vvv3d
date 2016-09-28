#ifndef ENGINE_H
#define ENGINE_H
#include <memory>

class ResourceManager;

class Engine
{
public:
    Engine(int argc, char** argv, const char* windowName = "vvv3d");
    virtual ~Engine();

    ResourceManager& getResourceManager();

    void run();
    float getCurrentFps() const;
    int getVieportWidth() const;
    int getVieportHeight() const;

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static std::shared_ptr<Engine>  activeEngine;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    int viewportWidth;
    int viewportHeight;
    std::unique_ptr<ResourceManager> resourceManager;
};
#endif // ENGINE

