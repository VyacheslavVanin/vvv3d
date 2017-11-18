#ifndef BUTTONTEXT_H
#define BUTTONTEXT_H
#include "buttonbase.h"

namespace vvv3d {

class TextWidget;
class ColorRectWidget;
class Panel;
class ButtonText : public ButtonBase {
public:
    ButtonText(const std::string& caption = "Button");
    ButtonText(const ButtonText&)         = delete;
    ButtonText& operator=(const ButtonText&) = delete;

    void setText(const std::string& text);
    const std::string& getText() const;

private:
    TextWidget* text;
    ColorRectWidget* background;
    Panel* panel;

    // ButtonBase interface
protected:
    void onHover() override;
    void onUnhover() override;
    void onPressDown() override;
    void onContentChanged() override;
};

}

#endif // BUTTONTEXT_H
