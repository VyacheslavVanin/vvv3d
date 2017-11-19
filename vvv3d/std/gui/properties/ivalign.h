#ifndef VVV_IVALIGN_H
#define VVV_IVALIGN_H
#include <vvv3d/std/gui/align.h>

namespace vvv3d {

class IVAlignProperty {
public:
    virtual void setVAlign(VALIGN align) = 0;
    virtual VALIGN getVAlign() const = 0;
    virtual ~IVAlignProperty() = default;
};

} // namespace vvv3d

#endif
