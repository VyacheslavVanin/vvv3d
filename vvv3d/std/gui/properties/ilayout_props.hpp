#ifndef VVV_ILAYOUT_PROPS_H
#define VVV_ILAYOUT_PROPS_H
#include "iborder.hpp"
#include "iexpand.hpp"
#include "ipadding.hpp"

namespace vvv3d {

class ILayoutProperties : public IBorderProperty,
                          public IPaddingProperty,
                          public IExpandProperty {
};

} // namespace vvv3d

#endif
