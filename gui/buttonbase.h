#ifndef BUTTONBASE_H
#define BUTTONBASE_H
#include "widget.h"

class ButtonBase : public Widget
{
public:
    ButtonBase() {}

private:
    enum class STATE {
        START,
        HOVER,
        HOVER_PRESSED,
        PRESSED_MOVEDOUT,
    };
    STATE state = STATE::START;
    // Widget interface
protected:
    virtual void onClicked();
    virtual void onHover();
    virtual void onUnhover();

    void OnButtonPressed(int button, int x, int y) override;
    void OnButtonReleased(int button, int x, int y) override;
    void OnPointerEnter(int x, int y) override;
    void OnPointerLeave(int x, int y) override;
};


#endif // BUTTONBASE_H
