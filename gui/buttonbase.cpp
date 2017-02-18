#include "buttonbase.h"

ButtonBase::ButtonBase() : actions() {setFocusable(true);}

void ButtonBase::addOnClickAction(const std::function<void ()>& action)
{
    actions.addAction(action);
}

void ButtonBase::onClicked() {actions.invoke();}

void ButtonBase::onHover()   {}

void ButtonBase::onUnhover() {}

void ButtonBase::onPressDown() {}

void ButtonBase::OnButtonPressed(int button, int x, int y)
{
    if(button != 1)
        return;
    switch(state) {
    case STATE::HOVER: state = STATE::HOVER_PRESSED; onPressDown(); break;
    default: break;
    }
    (void)x; (void)y;
}

void ButtonBase::OnButtonReleased(int button, int x, int y)
{
    if(button != 1)
        return;
    switch(state) {
    case STATE::HOVER_PRESSED: state = STATE::HOVER; onClicked(); onHover(); break;
    case STATE::HOVER: break;
    default: state = STATE::START; break;
    }
    (void)x; (void)y;
}

void ButtonBase::OnPointerEnter(int x, int y)
{
    onHover();
    switch(state) {
    case STATE::START:            state = STATE::HOVER;         break;
    case STATE::PRESSED_MOVEDOUT: state = STATE::HOVER_PRESSED; onPressDown(); break;
    default: break;
    }
    (void)x; (void)y;
}

void ButtonBase::OnPointerLeave(int x, int y)
{
    onUnhover();
    switch(state) {
    case STATE::HOVER:         state = STATE::START;            break;
    case STATE::HOVER_PRESSED: state = STATE::PRESSED_MOVEDOUT; break;
    default: break;
    }
    (void)x; (void)y;
}
