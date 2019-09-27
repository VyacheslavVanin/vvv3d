#include "buttonbase.hpp"

namespace vvv3d {

ButtonBase::ButtonBase() : actions() { setFocusable(true); }

void ButtonBase::addOnClickAction(const std::function<void()>& action)
{
    actions.addAction(action);
}

void ButtonBase::onHoverReleased(int, int)
{
    actions();
    invokeAction(actionNames);
}

void ButtonBase::setAction(const std::string& actionName)
{
    actionNames.push_back(actionName);
}

} // namespace vvv3d
