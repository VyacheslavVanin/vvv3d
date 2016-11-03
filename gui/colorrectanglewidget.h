#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "widget.h"
#include <memory>

class Color;

class ColorRectangleWidget final : public Widget
{
public:
    explicit ColorRectangleWidget(Widget* parent = nullptr);
    explicit ColorRectangleWidget(const Color& colour,
                                  Widget* parent = nullptr);
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
