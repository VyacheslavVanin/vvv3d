#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H
#include "widget.h"
#include <memory>
#include <string>

class Colour;
class TextWidget final : public Widget
{
public:
    TextWidget();
    TextWidget(const std::string& text = "Text");
    ~TextWidget();

    void setText(const std::string& text);
    void setColor(const Colour& color);

private:
    struct TextWidgetImpl;
    std::unique_ptr<TextWidgetImpl> pImpl;
    
    // Widget interface
protected:
    void onDraw() override;
};

#endif // TEXTWIDGET_H
