#ifndef LINEEDIT_H
#define LINEEDIT_H
#include "actionlist.h"
#include "align.h"
#include "colorrectanglewidget.h"
#include "textwidget.h"
#include "widget.h"
#include <string>
#include <chrono>
#include <vvv3d/core/graphics/color.h>

namespace vvv3d {

class LineEdit : public Widget {
public:
    LineEdit(const std::string& text = "");

    void setText(const std::string& text);
    void setFont(const vvv3d::Font& font);

    std::string getText() const;
    std::u32string getText32() const;

    void setHAlign(HALIGN align);
    HALIGN getHAlign() const;

    void setBackgroundColor(const vvv3d::Color& color);
    void setCursorColor(const vvv3d::Color& color);

    void
    addOnPressEnterAction(const std::function<void(const std::string&)>& f);

    // Widget interface
    void chargeBlink();

protected:
    void onGetFocus() override;
    void onLoseFocus() override;
    void onKeyDown(uint16_t scancode) override;

    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;

    void onTextEntered(const std::string& text) override;
    void onDraw() override;

private:
    ActionList<void(const std::string&)> onEnterPressedActions;

    TextWidget* leftpart        = nullptr;
    TextWidget* rightpart       = nullptr;
    ColorRectWidget* cursor     = nullptr;
    ColorRectWidget* background = nullptr;
    HALIGN hAlign;
    std::chrono::system_clock::time_point toggleCursorVisibilityThresholdTime;
    std::chrono::system_clock::duration toggleCursorVisiblityPeriod;

    void placeWidgets();
    int roughLeftOffset(int leftMargin, int width, int fullTextWidth,
                        int rightMargin) const;
    void adjustCursorHeight();
};

}

#endif
