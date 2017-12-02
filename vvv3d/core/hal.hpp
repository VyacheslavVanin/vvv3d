#pragma once
#include "inputevent.hpp"
#include <functional>
#include <string>
#include <vector>
#include <vvv3d/vvvmath/matrices_types.hpp>

namespace vvv3d {

enum class GLPROFILE { ES, CORE };

/**
 * @brief The HAL class.
 *  Interface that provide:<br>
 *  - initialization of OpenGL Context <br>
 *  - Main window creation <br>
 *  - callback functions (resize, display, idle)
 *  - main event loop
 *  - acces to input data (mouse, keyboard etc.)
 */
class HAL {
public:
    virtual void initContext(int argc, char** argv) = 0;
    virtual void createWindow(int width, int height, const char* name) = 0;
    virtual void setResizeFunction(
        const std::function<void(int x, int y)>& resizeFunction) = 0;
    virtual void
    setDisplayFunction(const std::function<void()>& displayFunction)        = 0;
    virtual void setIdleFunction(const std::function<void()>& idleFunction) = 0;
    virtual void mainLoop()                                                 = 0;
    virtual void swap()                                                     = 0;

    virtual size_t getNumScanCodes() const              = 0;
    virtual bool keyDown(uint16_t scancode) const       = 0;
    virtual bool mouseButtonDown(uint16_t button) const = 0;
    virtual int getMouseX() const                       = 0;
    virtual int getMouseY() const                       = 0;
    vvv::vector2i getMousePos() const;

    virtual int getMouseRelX() const = 0;
    virtual int getMouseRelY() const = 0;
    bool isMouseMoved() const;
    vvv::vector2i getMouseRel() const;

    virtual bool hasText() const                             = 0;
    virtual const std::string& getText() const               = 0;
    virtual const std::vector<InputEvent>& getEvents() const = 0;
    virtual void setVSync(bool vsync) = 0;
    virtual ~HAL();
};
}

