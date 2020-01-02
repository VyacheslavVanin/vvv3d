#include "sliderv.hpp"
#include <vvv3d/vvv3dgui.hpp>

namespace vvv3d {

SliderV::SliderV()
    : SliderBase(), background(new vvv3d::ColorRectWidget(vvv3d::WHITE)),
      pointer(new vvv3d::ColorRectWidget(vvv3d::BLUE)), pointerHeight(20)
{
    addChild(background);
    addChild(pointer);
    setSize(20, 120);
    onValueChanged();
    onRangeChanged();
}

void SliderV::onResize(const vvv::vector2i& oldSize,
                       const vvv::vector2i& newSize)
{
    (void)oldSize;
    background->setSize(newSize);
    pointer->setSize(newSize.x, pointerHeight);
    onValueChanged();
    onRangeChanged();
}

int SliderV::mousePosToValue(int x, int y)
{
    (void)x;
    const float posInActiveArea =
        y - getAbsolutePosition().y - pointer->getHeight() / 2;
    const float activeAreaWidth = getHeight() - pointer->getHeight();
    return getRange() * (1.0 - posInActiveArea / activeAreaWidth);
}

void SliderV::onValueChanged()
{
    const auto height = getHeight() - pointer->getHeight();
    const auto pos = float(getValue()) / getRange() * height;
    pointer->setPosition(0, height - pos);
}

void SliderV::onRangeChanged()
{
    pointer->setSize(getSize().x, pointerHeight);
    onValueChanged();
}

void SliderV::setColor(const vvv3d::Color& color)
{
    pointer_color = color;
    static_cast<ColorRectWidget*>(pointer)->setColor(color);
}

const vvv3d::Color& SliderV::getColor() const { return pointer_color; }

} // namespace vvv3d
