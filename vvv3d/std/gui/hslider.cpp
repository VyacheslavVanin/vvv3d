#include "hslider.hpp"
#include <vvv3d/vvv3dgui.hpp>

namespace vvv3d {

SliderH::SliderH()
    : SliderBase(), background(new vvv3d::ColorRectWidget(vvv3d::WHITE)),
      pointer(new vvv3d::ColorRectWidget(vvv3d::BLUE)), pointerWidth(20)
{
    addChild(background);
    addChild(pointer);
    setSize(120, 20);
    onValueChanged();
    onRangeChanged();
}

void SliderH::onResize(const vvv::vector2i& oldSize,
                       const vvv::vector2i& newSize)
{
    (void)oldSize;
    background->setSize(newSize);
    pointer->setSize(pointerWidth, newSize.y);
    onValueChanged();
    onRangeChanged();
}

int SliderH::mousePosToValue(int x, int y)
{
    (void)y;
    const float posInActiveArea =
        x - getAbsolutePosition().x - pointer->getWidth() / 2;
    const float activeAreaWidth = getWidth() - pointer->getWidth();
    return getRange() * posInActiveArea / activeAreaWidth;
}

void SliderH::onValueChanged()
{
    const auto pos =
        float(getValue()) / getRange() * (getWidth() - pointer->getWidth());
    pointer->setPosition(pos, 0);
}

void SliderH::onRangeChanged()
{
    pointer->setSize(pointerWidth, getSize().y);
    const auto pos =
        float(getValue()) / getRange() * (getWidth() - pointer->getWidth());
    pointer->setPosition(pos, 0);
}

void SliderH::setColor(const vvv3d::Color& color)
{
    pointer_color = color;
    static_cast<ColorRectWidget*>(pointer)->setColor(color);
}

const vvv3d::Color& SliderH::getColor() const { return pointer_color; }

} // namespace vvv3d
