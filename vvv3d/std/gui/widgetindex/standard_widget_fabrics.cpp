#include "standard_properties.hpp"

namespace vvv3d {

vvv3d::Panel* makePanel()
{
    static const vvv3d::Color defaultColor = vvv3d::Color(0.1, 0.8, 0.6, 0.5);
    auto layout = new vvv3d::VerticalLayout();
    auto background = new vvv3d::ColorRectWidget(defaultColor);
    layout->setBorder(15);
    layout->setExpandToContent(true);
    return new vvv3d::Panel(layout, background);
}

vvv3d::HorizontalLayout* makeHLayout() { return new vvv3d::HorizontalLayout(); }

vvv3d::VerticalLayout* makeVLayout() { return new vvv3d::VerticalLayout(); }

vvv3d::ButtonText* makeTextButton() { return new vvv3d::ButtonText("Button"); }

vvv3d::TextWidget* makeLabel() { return new vvv3d::TextWidget("Label"); }

vvv3d::LineEdit* makeLineEdit() { return new vvv3d::LineEdit(); }

vvv3d::SliderH* makeSliderH() { return new vvv3d::SliderH(); }

vvv3d::ImageWidget* makeImage() {
    auto& rm = vvv3d::Engine::getActiveEngine().getResourceManager();
    auto& tm = rm.getTextureManager();
    auto ret = new vvv3d::ImageWidget();
    ret->setTexture(tm.get("default"));
    return ret;
}
}
