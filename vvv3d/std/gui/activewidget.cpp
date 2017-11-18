#include "activewidget.h"

namespace vvv3d {

ActiveWidget::ActiveWidget() : Widget() {}

void ActiveWidget::onButtonPressed(int button, int x, int y)
{
    if (button != 1)
        return;

    switch (state) {
    case STATE::HOVER:
        state = STATE::HOVER_PRESSED;
        onHoverPressed(x, y);
        break;
    default: break;
    }
}

void ActiveWidget::onButtonReleased(int button, int x, int y)
{
    if (button != 1)
        return;
    switch (state) {
    case STATE::HOVER_PRESSED:
        state = STATE::HOVER;
        onHoverReleased(x, y);
        onHover();
        break;

    case STATE::HOVER: break;

    default: state = STATE::START; break;
    }
}

void ActiveWidget::onPointerEnter(int x, int y)
{
    onHover();
    switch (state) {
    case STATE::START: state = STATE::HOVER; break;
    case STATE::PRESSED_MOVED_OUTSIDE:
        state = STATE::HOVER_PRESSED;
        onHoverPressed(x, y);
        break;
    default: break;
    }
}

void ActiveWidget::onPointerLeave(int x, int y)
{
    onUnhover();
    switch (state) {
    case STATE::HOVER: state = STATE::START; break;
    case STATE::HOVER_PRESSED: state = STATE::PRESSED_MOVED_OUTSIDE; break;
    default: break;
    }
    (void)x;
    (void)y;
}

void ActiveWidget::onPointerMove(int x, int y)
{
    switch (state) {
    case STATE::HOVER_PRESSED: onMovePressedInside(x, y); break;
    case STATE::PRESSED_MOVED_OUTSIDE: onMovePressedOutside(x, y); break;
    default: break;
    }
}

void ActiveWidget::onHover() {}
void ActiveWidget::onUnhover() {}
void ActiveWidget::onHoverPressed(int, int) {}
void ActiveWidget::onMovePressedInside(int, int) {}
void ActiveWidget::onMovePressedOutside(int, int) {}
void ActiveWidget::onHoverReleased(int, int) {}

} // namespace vvv3d
