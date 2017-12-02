#include "buttonbase.hpp"

namespace vvv3d {

ButtonBase::ButtonBase() : actions() { setFocusable(true); }

void ButtonBase::addOnClickAction(const std::function<void()>& action)
{
    actions.addAction(action);
}

void ButtonBase::onHoverReleased(int, int) { actions.invoke(); }

} // namespace vvv3d
