#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H
#include "widget.h"
#include <memory>
#include <string>
#include "align.h"

class Color;
class Font;
class TextWidget: public Widget
{
public:
    explicit TextWidget(const std::string& text = "Text");
    ~TextWidget();

    void setText(const std::string& text);
    void setText(const std::u32string& text);
    void setText(std::u32string&& text);
    const std::u32string& getText() const;

    void append(const std::string& text);
    void append(const std::u32string& text);
    void append(char32_t character);
    void prepend(const std::string& text);
    void prepend(const std::u32string& text);
    void prepend(char32_t character);

    char32_t popBack();
    char32_t popFront();

    void setColor(const Color& color);
    void setFont(std::shared_ptr<Font> font);
    void resizeToContent();
    void setAutoSize(bool value);

    void setHAlign(HALIGN value);
    void setVAlign(VALIGN value);

private:
    struct TextWidgetImpl;
    std::unique_ptr<TextWidgetImpl> pImpl;
    bool autosize;
    HALIGN halign;
    VALIGN valign;

    // Widget interface
    void autoresize();

    int getHAlignOffset() const;
    
    int getVAlignOffset() const;

protected:
    void onDraw() override;
};

#endif // TEXTWIDGET_H
