#pragma once
#include <vvv3d/core/inputevent.h>
#include <vector>
#include <string>
#include <vvv3d/vvvmath/matrices_types.h>

namespace vvv3d {
class Input {
public:
    class Keyboard {
    public:
        bool keyDown(uint16_t scancode) const;
        bool hasText() const;
        const std::string& getText() const;
        const std::vector<InputEvent>& getEvents() const;
    };
    class Mouse {
    public:
        bool buttonDown(uint16_t button) const;
        vvv::vector2i getMousePos() const;
        vvv::vector2i getMouseRel() const;
        int getMouseX() const;
        int getMouseY() const;
        int getMouseRelX() const;
        int getMouseRelY() const;
        bool isMoved() const;
    };
    const Keyboard& getKeyboard() const;
    const Mouse& getMouse() const;

private:
    Keyboard keyboard;
    Mouse mouse;
};

}

