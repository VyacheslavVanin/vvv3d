#ifndef SDLLAYER_H
#define SDLLAYER_H
#include "hal.h"
#include <memory>

class sdlLayer : public HAL
{
public:
    sdlLayer(int argc, char** argv,
             GLPROFILE p = GLPROFILE::CORE,
             int major = 3, int minor = 3);

    void initContext(int argc, char** argv) override;
    void createWindow(int width, int height, const char* name) override;
    void mainLoop() override;
    void swap() override;

    void setResizeFunction(const std::function<void(int x, int y)>&
                           resizeFunction) override;

    void setDisplayFunction(const std::function<void()>&
                            displayFunction) override;

    void setIdleFunction(const std::function<void()>& idleFunction) override;

    ~sdlLayer();
private:
    class SDLWraper;
    std::unique_ptr<SDLWraper> sdl;

    // HAL interface
public:
    size_t getNumScanCodes() const override;
    bool keyDown(uint16_t scancode) const override;
    bool mouseButtonDown(uint16_t button) const override;
    int getMouseX() const override;
    int getMouseY() const override;
    int getMouseRelX() const override;
    int getMouseRelY() const override;
    bool hasText() const override;
    const std::string& getText() const override;
    const std::vector<input_event>& getEvents() const;
};


#endif // SDLLAYER_H
