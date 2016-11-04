#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H
#include "widget.h"

enum class HALIGN {
    LEFT, CENTER, RIGHT
};

class VerticalLayout : public Widget
{
public:
    VerticalLayout(Widget* parent=nullptr);

    int getBorder() const;
    void setBorder(int value);

    int getPadding() const;
    void setPadding(int value);

    HALIGN getAlign() const;
    void setAlign(HALIGN value);

    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);


protected:
    // Widget interface
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
    void onAddWidget(Widget* added);
    void onRemoveWidget(Widget* removed);
    void onContentChanged(Widget* changed);

private:
    int border;
    int padding;
    HALIGN align;
    int getMaxChildWidth(const std::vector<Widget*>& children);
    int getChildrenHeight(const std::vector<Widget*>& children);
    void rearrange();
};

#endif // VERTICALLAYOUT_H
