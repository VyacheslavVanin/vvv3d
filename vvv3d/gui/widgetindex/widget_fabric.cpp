#include "widget_fabric.hpp"
#include "standard_widget_fabrics.hpp"

namespace vvv3d {
template <>
Fabric<vvv3d::Widget*>::Fabric()
    : standard_map{
          {"panel", makePanel},     {"hlayout", makeHLayout},
          {"vlayout", makeVLayout}, {"button", makeTextButton},
          {"label", makeLabel},     {"lineedit", makeLineEdit},
          {"sliderh", makeSliderH}, {"image", makeImage},
      }
{
}

} // namespace vvv3d
