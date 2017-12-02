#pragma once
#include <vvv3d/std/gui/widget.hpp>

namespace vvv3d {

class ActiveWidget : public vvv3d::Widget {
public:
    ActiveWidget();

protected:
    virtual void onHover();
    virtual void onUnhover();
    virtual void onHoverPressed(int x, int y);
    virtual void onMovePressedInside(int x, int y);
    virtual void onMovePressedOutside(int x, int y);
    virtual void onHoverReleased(int x, int y);

    void onButtonPressed(int button, int x, int y) override;
    void onButtonReleased(int button, int x, int y) override;
    void onPointerEnter(int x, int y) override;
    void onPointerLeave(int x, int y) override;
    void onPointerMove(int x, int y) override;

private:
    enum class STATE {
        START,
        HOVER,
        HOVER_PRESSED,
        PRESSED_MOVED_OUTSIDE,
    };
    STATE state = STATE::START;
};

} // namespace vvv3d
