#ifndef BUTTONBASE_H
#define BUTTONBASE_H
#include "actionlist.h"
#include "widget.h"

namespace vvv3d {

class ButtonBase : public Widget {
public:
    ButtonBase();
    void addOnClickAction(const std::function<void(void)>& action);

private:
    enum class STATE {
        START,
        HOVER,
        HOVER_PRESSED,
        PRESSED_MOVEDOUT,
    };
    STATE state = STATE::START;

    ActionList<void(void)> actions;
    void onClicked();

    // Widget interface
protected:
    virtual void onHover();
    virtual void onUnhover();
    virtual void onPressDown();

    void onButtonPressed(int button, int x, int y) override;
    void onButtonReleased(int button, int x, int y) override;
    void onPointerEnter(int x, int y) override;
    void onPointerLeave(int x, int y) override;
};

}

#endif // BUTTONBASE_H
