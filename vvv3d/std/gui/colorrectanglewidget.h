#pragma once
#include "widget.h"
#include <memory>
#include <vvv3d/vvv3d.h>
#include <vvv3d/std/gui/properties/icolor.h>

namespace vvv3d {

class ColorRectWidget : public Widget, public IColorProperty {
public:
    explicit ColorRectWidget();
    explicit ColorRectWidget(const vvv3d::Color& colour);
    ~ColorRectWidget();

    void setColor(const vvv3d::Color& colour) override;
    const vvv3d::Color& getColor() const override;

private:
    vvv3d::Color color;

    // Widget interface
protected:
    void onDraw() override;
};

}

