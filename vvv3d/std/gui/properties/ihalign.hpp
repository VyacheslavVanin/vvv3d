#ifndef VVV_IHALIGN_H
#define VVV_IHALIGN_H
#include <vvv3d/std/gui/align.hpp>

namespace vvv3d {

class IHAlignProperty {
public:
    virtual void setHAlign(HALIGN align) = 0;
    virtual HALIGN getHAlign() const = 0;
    virtual ~IHAlignProperty() = default;
};

} // namespace vvv3d

#endif
