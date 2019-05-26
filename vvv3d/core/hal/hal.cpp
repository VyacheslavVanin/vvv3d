#include "hal.hpp"
#include <iostream>
#include <vvv3d/vvvmath/linalg.hpp>

using namespace vvv3d;

vvv::vector2i HAL::getMousePos() const
{
    return vvv::vector2i(getMouseX(), getMouseY());
}

bool HAL::isMouseMoved() const
{
    return getMouseRelX() != 0 || getMouseRelY() != 0;
}

vvv::vector2i HAL::getMouseRel() const
{
    return vvv::vector2i(getMouseRelX(), getMouseRelY());
}

void HAL::showVirtualKeyboard(bool){};

void HAL::log(const char* message) { std::cerr << message << "\n"; }

HAL::~HAL() {}
