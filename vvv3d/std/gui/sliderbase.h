#ifndef SLIDERBASE_H
#define SLIDERBASE_H
#include <vvv3d/std/gui/activewidget.h>
#include <vvv3d/std/gui/actionlist.h>

namespace vvv3d {

class SliderBase : public ActiveWidget
{
    public:
        SliderBase();
        void setRange(int max);
        int getMax() const;

        void setValue(int value);
        int getValue() const;

        void addOnValueChangedAction(const std::function<void(int)>& action);

    protected:
        void onHoverPressed(int x, int y) override;
        void onMovePressedInside(int x, int y) override;
        void onMovePressedOutside(int x, int y) override;

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
