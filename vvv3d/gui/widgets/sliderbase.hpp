#pragma once
#include <string>
#include <vector>
#include <vvv3d/gui/actionlist.hpp>
#include <vvv3d/gui/properties/iaction.hpp>
#include <vvv3d/gui/properties/irange.hpp>
#include <vvv3d/gui/properties/ivalue.hpp>
#include <vvv3d/gui/widgets/activewidget.hpp>

namespace vvv3d {

class SliderBase : public ActiveWidget,
                   public IValueProperty,
                   public IRangeProperty,
                   public IOnValueChangeProperty {
public:
    SliderBase();
    void setRange(int max) override;
    int getRange() const override;

    void setValue(int value) override;
    int getValue() const override;

    void addOnValueChangedAction(const std::function<void(int)>& action);

    void setOnValueChangedAction(const std::string& action_name) override;

protected:
    void onHoverPressed(int x, int y) override;
    void onMovePressedInside(int x, int y) override;
    void onMovePressedOutside(int x, int y) override;
    void onKeyDown(uint16_t scancode) override;

    /// Calculate slider value from pointer position
    virtual int mousePosToValue(int x, int y) = 0;

    /// Update widget view on value change
    virtual void onValueChanged() = 0;
    virtual void onRangeChanged() = 0;

private:
    vvv3d::ActionList<void(int)> onValueChangedActions;
    std::vector<std::string> onValueChangeActionNames;
    int max;
    int value;

    void changeValueMotion(int x, int y);
};

} // namespace vvv3d
