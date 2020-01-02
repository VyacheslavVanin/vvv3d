#pragma once
#include "sliderbase.hpp"
#include <vvv3d/gui/properties/icolor.hpp>

namespace vvv3d {

class SliderH : public SliderBase, public IColorProperty {
public:
    SliderH();

protected:
    int mousePosToValue(int x, int y) override;
    void onValueChanged() override;
    void onRangeChanged() override;

    void setColor(const vvv3d::Color& color) override;
    const vvv3d::Color& getColor() const override;

    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;

private:
    vvv3d::Widget* background;
    vvv3d::Widget* pointer;
    int pointerWidth;
    vvv3d::Color pointer_color;
};

} // namespace vvv3d
