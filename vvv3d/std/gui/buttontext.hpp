#pragma once
#include "buttonbase.hpp"
#include "properties/itext.hpp"

namespace vvv3d {

class TextWidget;
class ColorRectWidget;
class Panel;
class ButtonText : public ButtonBase, public ITextProperty {
public:
    ButtonText(const std::string& caption = "Button");
    ButtonText(const ButtonText&) = delete;
    ButtonText& operator=(const ButtonText&) = delete;

    void setText(const std::string& text) override;
    const std::string& getText() const override;

private:
    TextWidget* text;
    ColorRectWidget* background;
    Panel* panel;

    // ButtonBase interface
protected:
    void onHover() override;
    void onUnhover() override;
    void onHoverPressed(int x, int y) override;
    void onContentChanged() override;
};

} // namespace vvv3d
