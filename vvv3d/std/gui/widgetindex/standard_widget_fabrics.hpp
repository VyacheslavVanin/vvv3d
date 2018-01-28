#pragma once
#include <vvv3d/vvv3dgui.hpp>

namespace vvv3d {
vvv3d::Panel* makePanel();
vvv3d::HorizontalLayout* makeHLayout();
vvv3d::VerticalLayout* makeVLayout();
vvv3d::ButtonText* makeTextButton();
vvv3d::TextWidget* makeLabel();
vvv3d::LineEdit* makeLineEdit();
vvv3d::SliderH* makeSliderH();
vvv3d::ImageWidget* makeImage();
} // namespace vvv3d
