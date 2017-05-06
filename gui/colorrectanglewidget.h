#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "widget.h"
#include <memory>
#include <vvv3d/vvv3d.h>

class ColorRectWidget : public Widget {
public:
    explicit ColorRectWidget();
    explicit ColorRectWidget(const Color& colour);
    ~ColorRectWidget();

    void setColor(const Color& colour);

private:
    Color color;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // COLORRECTANGLEWIDGET_H
