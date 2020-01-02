#pragma once
#include "layout.hpp"
#include <vvv3d/gui/align.hpp>
#include <vvv3d/gui/properties/ihalign.hpp>
#include <vvv3d/gui/widgets/widget.hpp>

namespace vvv3d {

class VerticalLayout : public Layout, public IHAlignProperty {
public:
    VerticalLayout();
    HALIGN getHAlign() const override;
    void setHAlign(HALIGN value) override;

protected:
    // Layout interface
    void rearrange() override;

private:
    int getMaxChildWidth(const std::vector<Widget*>& children);
    int getChildrenHeight(const std::vector<Widget*>& children);
    HALIGN align;
};

} // namespace vvv3d
