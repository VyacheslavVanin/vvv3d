#ifndef COLORRECTANGLEWIDGET_H
#define COLORRECTANGLEWIDGET_H
#include "guiobject.h"
#include <memory>

class Colour;

class ColorRectangleWidget final : public Widget
{
public:
    ColorRectangleWidget(const Colour& colour);
    ColorRectangleWidget();
    ~ColorRectangleWidget();

    void setColour(const Colour& colour);

private:
    struct ColorRectangleWidgetImpl;
    std::unique_ptr<ColorRectangleWidgetImpl> pImpl;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // COLORRECTANGLEWIDGET_H
