#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H
#include "widget.h"
#include <memory>
#include <string>

class Color;
class TextWidget final : public Widget
{
public:
    explicit TextWidget();
    explicit TextWidget(const std::string& text = "Text");
    ~TextWidget();

    void setText(const std::string& text);
    void setColor(const Color& color);

private:
    struct TextWidgetImpl;
    std::unique_ptr<TextWidgetImpl> pImpl;
    
    // Widget interface
protected:
    void onDraw() override;
};

#endif // TEXTWIDGET_H
