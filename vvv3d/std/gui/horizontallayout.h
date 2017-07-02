#ifndef HORIZONTALLAYOUT_H
#define HORIZONTALLAYOUT_H
#include "align.h"
#include "layout.h"
#include "widget.h"

namespace vvv3d {

class HorizontalLayout : public Layout {
public:
    HorizontalLayout();
    HALIGN getAlign() const;
    void setAlign(HALIGN value);

protected:
    // Layout interface
    void rearrange() override;

private:
    int getMaxChildHeight(const std::vector<Widget*>& children);
    int getChildrenWidth(const std::vector<Widget*>& children);
    HALIGN align;
};

}

#endif // HORIZONTALLAYOUT_H
