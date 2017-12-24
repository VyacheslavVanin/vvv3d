#pragma once
#include "actionlist.hpp"
#include "activewidget.hpp"
#include "properties/iaction.hpp"

namespace vvv3d {

class ButtonBase : public ActiveWidget, public IActionProperty {
public:
    ButtonBase();
    void addOnClickAction(const std::function<void(void)>& action);

    void setAction(const std::string&) override;

private:
    ActionList<void(void)> actions;
    std::vector<std::string> actionNames;

    // ActiveWidget interface
protected:
    void onHoverReleased(int x, int y) override;
};

} // namespace vvv3d
