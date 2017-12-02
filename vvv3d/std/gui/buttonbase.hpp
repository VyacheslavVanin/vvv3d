#pragma once
#include "actionlist.hpp"
#include "activewidget.hpp"

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

} // namespace vvv3d
