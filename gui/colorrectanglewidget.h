#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "widget.h"
#include <memory>

class Color;

class ColorRectWidget final : public Widget
{
public:
    explicit ColorRectWidget();
    explicit ColorRectWidget(const Color& colour);
    ~ColorRectWidget();

    void setColor(const Color& colour);

private:
    struct ColorRectangleWidgetImpl;
    std::unique_ptr<ColorRectangleWidgetImpl> pImpl;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // COLORRECTANGLEWIDGET_H
