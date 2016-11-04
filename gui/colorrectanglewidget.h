#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "widget.h"
#include <memory>

class Color;

class ColorRectangleWidget final : public Widget
{
public:
    explicit ColorRectangleWidget();
    explicit ColorRectangleWidget(const Color& colour);
    ~ColorRectangleWidget();

    void setColour(const Color& colour);

private:
    struct ColorRectangleWidgetImpl;
    std::unique_ptr<ColorRectangleWidgetImpl> pImpl;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // COLORRECTANGLEWIDGET_H
