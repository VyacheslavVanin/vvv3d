#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <string>
#include <vvv3d/core/graphics/color.h>
#include "widget.h"
#include "colorrectanglewidget.h"
#include "textwidget.h"
#include "actionlist.h"
#include "align.h"

class LineEdit : public Widget
{
public:
    LineEdit(const std::string& text="");

    void setText(const std::string& text);

    std::string getText() const;

    void setHAlign(HALIGN align);
    HALIGN getHAlign() const;

    void setBackgroundColor(const Color& color);
    void setCursorColor(const Color& color);

    void addOnPressEnterAction(const std::function<void(const std::string&)>& f);

    // Widget interface
protected:
    void OnGetFocus() override;
    void OnLoseFocus() override;
    void OnKeyDown(uint16_t scancode) override;

    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;

    void OnTextEntered(const std::string& text) override;

private:
    ActionList<void(const std::string&)> onTextChangedActions;
    ActionList<void(const std::string&)> onEnterPressedActions;

    TextWidget*      leftpart   = nullptr;
    TextWidget*      rightpart  = nullptr;
    ColorRectWidget* cursor     = nullptr;
    ColorRectWidget* background = nullptr;
    HALIGN           hAlign;

    void placeWidgets();
};

#endif

