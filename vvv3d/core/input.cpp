#include "input.hpp"
#include <core/hal/hal.hpp>
#include <core/resourcemanager.hpp>
#include <vvv3d/core/engine.hpp>

namespace vvv3d {

const Input::Keyboard& Input::getKeyboard() const { return keyboard; }

const Input::Mouse& Input::getMouse() const { return mouse; }

bool Input::Keyboard::keyDown(uint16_t scancode) const
{
    return Engine::getActiveEngine().hal->keyDown(scancode);
}

bool Input::Keyboard::hasKeyDownEvent(uint16_t scancode) const
{
    const auto& events = getEvents();
    for (const auto& e : events)
        if (e.scancode == scancode &&
            e.type == vvv3d::INPUT_EVENT_TYPE::KEY_DOWN)
            return true;
    return false;
}

bool Input::Keyboard::hasKeyUpEvent(uint16_t scancode) const
{
    const auto& events = getEvents();
    for (const auto& e : events)
        if (e.scancode == scancode && e.type == vvv3d::INPUT_EVENT_TYPE::KEY_UP)
            return true;
    return false;
}

bool Input::Keyboard::hasText() const
{
    return Engine::getActiveEngine().hal->hasText();
}

const std::string& Input::Keyboard::getText() const
{
    return Engine::getActiveEngine().hal->getText();
}

const std::vector<InputEvent>& Input::Keyboard::getEvents() const
{
    return Engine::getActiveEngine().hal->getEvents();
}

bool Input::Mouse::buttonDown(uint16_t button) const
{
    return Engine::getActiveEngine().hal->mouseButtonDown(button);
}

vvv::vector2i Input::Mouse::getMousePos() const
{
    return vvv::vector2i(getMouseX(), getMouseY());
}

vvv::vector2i Input::Mouse::getMouseRel() const
{
    return vvv::vector2i(getMouseRelX(), getMouseRelY());
}

int Input::Mouse::getMouseX() const
{
    return Engine::getActiveEngine().hal->getMouseX();
}

int Input::Mouse::getMouseY() const
{
    return Engine::getActiveEngine().hal->getMouseY();
}

int Input::Mouse::getMouseRelX() const
{
    return Engine::getActiveEngine().hal->getMouseRelX();
}

int Input::Mouse::getMouseRelY() const
{
    return Engine::getActiveEngine().hal->getMouseRelY();
}

bool Input::Mouse::isMoved() const
{
    return Engine::getActiveEngine().hal->isMouseMoved();
}
} // namespace vvv3d
