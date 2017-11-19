#ifndef SLIDERBASE_H
#define SLIDERBASE_H
#include <vvv3d/std/gui/activewidget.h>
#include <vvv3d/std/gui/actionlist.h>
#include <vvv3d/std/gui/properties/irange.h>
#include <vvv3d/std/gui/properties/ivalue.h>

namespace vvv3d {

class SliderBase : public ActiveWidget, public IValueProperty, public IRangeProperty
{
    public:
        SliderBase();
        void setRange(int max) override;
        int getRange() const override;

        void setValue(int value) override;
        int getValue() const override;

        void addOnValueChangedAction(const std::function<void(int)>& action);

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
        int max;
        int value;

        void changeValueMotion(int x, int y);
};

}

#endif
