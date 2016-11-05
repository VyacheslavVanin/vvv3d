#ifndef PANEL_H
#define PANEL_H
#include "widget.h"
#include "layout.h"

class Panel : public Widget
{
public:
    Panel();
    Panel(Layout* layout, Widget* background);

    void addWidget(Widget* added);
    void removeWidget(Widget* removed);

    // Widget interface
protected:
    void onResize(const vvv::vector2i&,
                  const vvv::vector2i& newSize) override;
private:
    Layout* layout;
    Widget* background;

    void rearrange();
};


#endif // PANEL_H
