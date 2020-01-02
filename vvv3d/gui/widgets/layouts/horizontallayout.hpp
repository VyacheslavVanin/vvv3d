#pragma once
#include "layout.hpp"
#include <vvv3d/gui/align.hpp>
#include <vvv3d/gui/properties/ivalign.hpp>
#include <vvv3d/gui/widgets/widget.hpp>

namespace vvv3d {

class HorizontalLayout : public Layout, public IVAlignProperty {
public:
    HorizontalLayout();
    VALIGN getVAlign() const override;
    void setVAlign(VALIGN value) override;

protected:
    // Layout interface
    void rearrange() override;

private:
    int getMaxChildHeight(const std::vector<Widget*>& children);
    int getChildrenWidth(const std::vector<Widget*>& children);
    VALIGN align;
};

} // namespace vvv3d
