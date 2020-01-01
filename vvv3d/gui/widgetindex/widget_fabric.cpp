#include "widget_fabric.hpp"
#include "standard_widget_fabrics.hpp"

namespace vvv3d {
template<>
Fabric<Widget*>::Fabric() : standard_map{
    {"panel", makePanel},     {"hlayout", makeHLayout},
    {"vlayout", makeVLayout}, {"button", makeTextButton},
    {"label", makeLabel},     {"lineedit", makeLineEdit},
    {"sliderh", makeSliderH}, {"sliderv", makeSliderV},
    {"image", makeImage},
}, custom_map()
{
}

} // namespace vvv3d
