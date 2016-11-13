#include "hal.h"
#include <vvv3d/vvvmath/linalg.h>


vvv::vector2i HAL::getMousePos() const
{
    return vvv::vector2i(getMouseX(), getMouseY());
}

vvv::vector2i HAL::getMouseRel() const
{
    return vvv::vector2i(getMouseRelX(), getMouseRelY());
}
