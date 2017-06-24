#ifndef LAYOUT_H
#define LAYOUT_H
#include "widget.h"

class Layout : public Widget {
public:
    Layout();

    int getBorder() const;
    void setBorder(int value);

    int getPadding() const;
    void setPadding(int value);

    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);

    void setExpandToFitContent(bool expand);
    bool isExpandToFitContent() const;

protected:
    // Widget interface
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
    void OnPointerMove(int x, int y) override;
    void OnButtonPressed(int button, int x, int y) override;
    void OnButtonReleased(int button, int x, int y) override;

    void onAddWidget(Widget* added);
    void onRemoveWidget(Widget* removed);
    void onContentChanged(Widget* changed);
    virtual void rearrange() = 0;

private:
    int border;
    int padding;
    bool expandToFit = true;
};

#endif // LAYOUT_H
