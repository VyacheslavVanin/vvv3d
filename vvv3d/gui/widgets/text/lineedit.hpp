#pragma once
#include "textwidget.hpp"
#include <chrono>
#include <string>
#include <vector>
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/gui/actionlist.hpp>
#include <vvv3d/gui/align.hpp>
#include <vvv3d/gui/properties/iaction.hpp>
#include <vvv3d/gui/properties/ibgcolor.hpp>
#include <vvv3d/gui/properties/icursorcolor.hpp>
#include <vvv3d/gui/properties/ifont.hpp>
#include <vvv3d/gui/properties/ihalign.hpp>
#include <vvv3d/gui/properties/itext.hpp>
#include <vvv3d/gui/widgets/colorrectanglewidget.hpp>
#include <vvv3d/gui/widgets/widget.hpp>

namespace vvv3d {

class LineEdit : public Widget,
                 public ITextProperty,
                 public IFontProperty,
                 public IHAlignProperty,
                 public IBGColorProperty,
                 public ICursorColorProperty,
                 public IColorProperty,
                 public IOnEnterProperty,
                 public IOnValueChangeProperty {
public:
    LineEdit(const std::string& text = "");

    void setText(const std::string& text) override;
    const std::string& getText() const override;

    void setFont(const vvv3d::Font& font) override;
    const vvv3d::Font& getFont() const override;

    void setHAlign(HALIGN align) override;
    HALIGN getHAlign() const override;

    void setBGColor(const vvv3d::Color& color) override;
    const Color& getBGColor() const override;

    void setColor(const vvv3d::Color& color) override;
    const Color& getColor() const override;

    void setCursorColor(const vvv3d::Color& color) override;
    const vvv3d::Color& getCursorColor() const override;

    void setOnEnterAction(const std::string& action_name) override;
    void setOnValueChangedAction(const std::string& action_name) override;

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
    std::vector<std::string> onEnterActionNames;
    std::vector<std::string> onValueChangedActionNames;

    TextWidget* leftpart = nullptr;
    TextWidget* rightpart = nullptr;
    ColorRectWidget* cursor = nullptr;
    ColorRectWidget* background = nullptr;
    HALIGN hAlign;
    std::chrono::system_clock::time_point toggleCursorVisibilityThresholdTime;
    std::chrono::system_clock::duration toggleCursorVisiblityPeriod;
    mutable std::string text;
    mutable bool text_changed;

    void placeWidgets();
    int roughLeftOffset(int leftMargin, int width, int fullTextWidth,
                        int rightMargin) const;
    void adjustCursorHeight();

    void lazyUpdateText() const;
};

} // namespace vvv3d
