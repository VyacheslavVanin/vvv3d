#pragma once
#include <vvv3d/gui/align.hpp>

namespace vvv3d {

class IVAlignProperty {
public:
    virtual void setVAlign(VALIGN align) = 0;
    virtual VALIGN getVAlign() const = 0;
    virtual ~IVAlignProperty() = default;
};

} // namespace vvv3d
