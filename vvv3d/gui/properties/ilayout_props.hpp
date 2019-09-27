#pragma once
#include "iborder.hpp"
#include "iexpand.hpp"
#include "ipadding.hpp"

namespace vvv3d {

class ILayoutProperties : public IBorderProperty,
                          public IPaddingProperty,
                          public IExpandProperty {
};

} // namespace vvv3d
