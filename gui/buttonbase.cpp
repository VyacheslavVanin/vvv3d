#include "buttonbase.h"

void ButtonBase::addOnClickAction(const std::function<void ()>& action)
{
    actions.addAction(action);
}

void ButtonBase::onClicked() {actions.invoke();}

void ButtonBase::onHover()   {}

void ButtonBase::onUnhover() {}

void ButtonBase::OnButtonPressed(int button, int x, int y)
{
    if(button != 1)
        return;
    switch(state) {
    case STATE::HOVER: state = STATE::HOVER_PRESSED; break;
    default: break;
    }
}

void ButtonBase::OnButtonReleased(int button, int x, int y)
{
    if(button != 1)
        return;
    switch(state) {
    case STATE::HOVER_PRESSED: state = STATE::HOVER; onClicked(); break;
    case STATE::HOVER: break;
    default: state = STATE::START; break;
    }
}

void ButtonBase::OnPointerEnter(int x, int y)
{
    onHover();
    switch(state) {
    case STATE::START:            state = STATE::HOVER;         break;
    case STATE::PRESSED_MOVEDOUT: state = STATE::HOVER_PRESSED; break;
    default: break;
    }
}

void ButtonBase::OnPointerLeave(int x, int y)
{
    onUnhover();
    switch(state) {
    case STATE::HOVER:         state = STATE::START;            break;
    case STATE::HOVER_PRESSED: state = STATE::PRESSED_MOVEDOUT; break;
    default: break;
    }
}
