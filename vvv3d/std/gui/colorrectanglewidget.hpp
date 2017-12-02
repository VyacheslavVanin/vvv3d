#pragma once
#include "widget.hpp"
#include <memory>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/std/gui/properties/icolor.hpp>

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

