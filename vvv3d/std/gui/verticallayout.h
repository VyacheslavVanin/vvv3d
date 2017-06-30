#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H
#include "align.h"
#include "layout.h"
#include "widget.h"

class VerticalLayout : public Layout {
public:
    VerticalLayout();
    HALIGN getAlign() const;
    void setAlign(HALIGN value);

protected:
    // Layout interface
    void rearrange() override;

private:
    int getMaxChildWidth(const std::vector<Widget*>& children);
    int getChildrenHeight(const std::vector<Widget*>& children);
    HALIGN align;
};

#endif // VERTICALLAYOUT_H
