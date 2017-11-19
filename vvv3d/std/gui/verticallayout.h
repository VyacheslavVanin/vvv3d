#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H
#include "align.h"
#include "layout.h"
#include "widget.h"
#include "properties/ihalign.h"

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

}

#endif // VERTICALLAYOUT_H
