#pragma once
#include "actionlist.h"
#include "activewidget.h"

namespace vvv3d {

class ButtonBase : public ActiveWidget {
public:
    ButtonBase();
    void addOnClickAction(const std::function<void(void)>& action);

private:
    ActionList<void(void)> actions;

    // ActiveWidget interface
protected:
    void onHoverReleased(int x, int y) override;
};

}

