#ifndef BUTTONBASE_H
#define BUTTONBASE_H
#include "actionlist.h"
#include "widget.h"

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
    void                   onClicked();

    // Widget interface
protected:
    virtual void onHover();
    virtual void onUnhover();
    virtual void onPressDown();

    void OnButtonPressed(int button, int x, int y) override;
    void OnButtonReleased(int button, int x, int y) override;
    void OnPointerEnter(int x, int y) override;
    void OnPointerLeave(int x, int y) override;
};

#endif // BUTTONBASE_H
