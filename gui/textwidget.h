#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H
#include "widget.h"
#include <memory>
#include <string>

class Color;
class Font;
class TextWidget: public Widget
{
public:
    explicit TextWidget(const std::string& text = "Text");
    ~TextWidget();

    void setText(const std::string& text);
    void setColor(const Color& color);
    void setFont(std::shared_ptr<Font> font);
    void resizeToContent();
    void setAutoSize(bool value);

private:
    struct TextWidgetImpl;
    std::unique_ptr<TextWidgetImpl> pImpl;
    bool autosize;

    // Widget interface
    void autoresize();

protected:
    void onDraw() override;
};

#endif // TEXTWIDGET_H
