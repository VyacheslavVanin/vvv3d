#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H
#include "widget.h"

enum class HALIGN {
    LEFT, CENTER, RIGHT
};

class VerticalLayout : public Widget
{
public:
    VerticalLayout();

    int getBorder() const;
    void setBorder(int value);

    int getPadding() const;
    void setPadding(int value);

    HALIGN getAlign() const;
    void setAlign(HALIGN value);

protected:
    // Widget interface
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
    void onAddWidget(Widget* added) override;
    void onRemoveWidget(Widget* removed) override;
    void onContentChanged(Widget* changed) override;

private:
    int border;
    int padding;
    HALIGN align;
    int getMaxChildWidth(const std::vector<Widget*>& children);
    int getChildrenHeight(const std::vector<Widget*>& children);
    void rearrange();
};

#endif // VERTICALLAYOUT_H
