#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "widget.h"
#include <memory>
#include <vvv3d/vvv3d.h>

class ColorRectWidget : public Widget {
public:
    explicit ColorRectWidget();
    explicit ColorRectWidget(const vvv3d::Color& colour);
    ~ColorRectWidget();

    void setColor(const vvv3d::Color& colour);

private:
    vvv3d::Color color;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // COLORRECTANGLEWIDGET_H
