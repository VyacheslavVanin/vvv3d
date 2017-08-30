#include "sliderbase.h"
#include <vvv3d/vvv3d.h>

namespace vvv3d {

SliderBase::SliderBase()
    : ActiveWidget(), onValueChangedActions(), max(100), value(max / 2)
{
    setFocusable(true);
}

void SliderBase::setRange(int max)
{
    this->max = std::max(max, 1);
    if (this->max < getValue())
        setValue(max);
    onRangeChanged();
}

int SliderBase::getMax() const
{
    return max;
}

void SliderBase::setValue(int value)
{
    const int newValue = vvv::clamp_fast(0, max, value);
    if (this->value == newValue)
        return;
    this->value = newValue;
    onValueChanged();
    onValueChangedActions.invoke(this->value);
}

int SliderBase::getValue() const
{
    return value;
}

void SliderBase::addOnValueChangedAction(const std::function<void(int)>& action)
{
    onValueChangedActions.addAction(action);
}

void SliderBase::changeValueMotion(int x, int y)
{
    const auto v = mousePosToValue(x, y);
    setValue(v);
}

void SliderBase::onHoverPressed(int x, int y)
{
    changeValueMotion(x, y);
}

void SliderBase::onMovePressedInside(int x, int y)
{
    changeValueMotion(x, y);
}

void SliderBase::onMovePressedOutside(int x, int y)
{
    changeValueMotion(x, y);
}

void SliderBase::onKeyDown(uint16_t scancode)
{
    const auto current = getValue();
    switch (scancode) {
        case SCANCODE_DOWN:
        case SCANCODE_LEFT: setValue(current - 1); break;
        case SCANCODE_UP:
        case SCANCODE_RIGHT:setValue(current + 1); break;
        default: break;
    }
}

}

