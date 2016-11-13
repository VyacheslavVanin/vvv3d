#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <vvv3d/vvvmath/matrices_types.h>

class ResourceManager;
class HAL;

class Engine
{
public:
    class Input;

    Engine(int argc, char** argv, const char* windowName = "vvv3d");
    virtual ~Engine();

    ResourceManager& getResourceManager();
    const Input& getInput() const;

    void run();
    float getCurrentFps() const;
    int getVieportWidth() const;
    int getVieportHeight() const;
    static Engine& getActiveEngine();

    class Input
    {
    public:
        class Keyboard
        {
        public:
            bool keyDown(uint16_t scancode) const;
            bool hasText() const;
            const std::string& getText() const;
        };
        class Mouse
        {
        public:
            bool buttonDown(uint16_t button) const;
            vvv::vector2i getMousePos() const;
            vvv::vector2i getMouseRel() const;
            int getMouseX() const;
            int getMouseY() const;
            int getMouseRelX() const;
            int getMouseRelY() const;
        };
        const Keyboard& getKeyboard() const;
        const Mouse& getMouse() const;

    private:
        Keyboard keyboard;
        Mouse    mouse;
    };

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static Engine*  activeEngine;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    int viewportWidth;
    int viewportHeight;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<HAL> hal;
    Input input;
};
#endif // ENGINE

