#include "hal.h"
#include <vvv3d/vvvmath/linalg.h>

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

HAL::~HAL() {}
