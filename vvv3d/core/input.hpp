#pragma once
#include <string>
#include <vector>
#include <vvv3d/core/inputevent.hpp>
#include <vvv3d/vvvmath/matrices_types.hpp>

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

} // namespace vvv3d
