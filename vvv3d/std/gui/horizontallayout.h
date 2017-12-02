#pragma once
#include "align.h"
#include "layout.h"
#include "widget.h"
#include "properties/ivalign.h"

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

}

