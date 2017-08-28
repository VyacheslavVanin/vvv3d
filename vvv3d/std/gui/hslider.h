#ifndef HSLIDER_H
#define HSLIDER_H
#include "sliderbase.h"

namespace vvv3d { 

class SliderH : public SliderBase {
public:
    SliderH();

protected:
    int mousePosToValue(int x, int y) override;
    void onValueChanged() override;
    void onRangeChanged() override;

    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;

private:
    vvv3d::Widget* background;
    vvv3d::Widget* pointer;
    int pointerWidth;
};

}

#endif
