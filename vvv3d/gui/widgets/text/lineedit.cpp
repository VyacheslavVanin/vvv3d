#include "lineedit.hpp"
#include <vvv3d/core/hal/hal.hpp>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvvmath/functions.hpp>

namespace vvv3d {

void LineEdit::adjustCursorHeight()
{
    const auto& font = leftpart->getFont();
    cursor->setSize(3, font.getHeight());
}

LineEdit::LineEdit(const std::string& text)
    : Widget(), hAlign(HALIGN::LEFT), toggleCursorVisibilityThresholdTime(),
      toggleCursorVisiblityPeriod(std::chrono::milliseconds(750)), text(),
      text_changed(true)
{
    auto lp = std::make_unique<TextWidget>(text);
    auto rp = std::make_unique<TextWidget>("");
    auto cur = std::make_unique<ColorRectWidget>(BLACK);
    auto bg = std::make_unique<ColorRectWidget>(WHITE);

    leftpart = lp.release();
    rightpart = rp.release();
    cursor = cur.release();
    background = bg.release();
    leftpart->setAutoResize(true);
    rightpart->setAutoResize(true);
    addChild(background);
    addChild(leftpart);
    addChild(rightpart);
    addChild(cursor);
    setFocusable(true);

    adjustCursorHeight();
    leftpart->setColor(BLACK);
    rightpart->setColor(BLACK);
    cursor->setVisible(false);
    setHeight(leftpart->getFont().getHeight());
}

void LineEdit::setText(const std::string& text)
{
    leftpart->setText(text);
    rightpart->setText("");
    leftpart->minimize();
    rightpart->minimize();
    placeWidgets();
    text_changed = true;
}

void LineEdit::setFont(const vvv3d::Font& font)
{
    leftpart->setFont(font);
    rightpart->setFont(font);
    adjustCursorHeight();
    placeWidgets();
}

const vvv3d::Font& LineEdit::getFont() const { return leftpart->getFont(); }

const std::string& LineEdit::getText() const
{
    lazyUpdateText();
    return text;
}

void LineEdit::setHAlign(HALIGN align)
{
    this->hAlign = align;
    placeWidgets();
}

HALIGN LineEdit::getHAlign() const { return this->hAlign; }

void LineEdit::setBGColor(const Color& color) { background->setColor(color); }

const Color& LineEdit::getBGColor() const { return background->getColor(); }

void LineEdit::setColor(const vvv3d::Color& color)
{
    leftpart->setColor(color);
    rightpart->setColor(color);
}

const Color& LineEdit::getColor() const { return leftpart->getColor(); }

void LineEdit::setCursorColor(const Color& color) { cursor->setColor(color); }

const vvv3d::Color& LineEdit::getCursorColor() const
{
    return cursor->getColor();
}

void LineEdit::addOnPressEnterAction(
    const std::function<void(const std::string&)>& f)
{
    onEnterPressedActions.addAction(f);
}

void LineEdit::chargeBlink()
{
    using namespace std::chrono;
    toggleCursorVisibilityThresholdTime =
        system_clock::now() + toggleCursorVisiblityPeriod;
    cursor->setVisible(true);
}

void LineEdit::onGetFocus()
{
    chargeBlink();
    vvv3d::Engine::getActiveEngine().getHAL().showVirtualKeyboard(true);
}

void LineEdit::onLoseFocus()
{
    using namespace std::chrono;
    toggleCursorVisibilityThresholdTime = system_clock::time_point();
    cursor->setVisible(false);
    vvv3d::Engine::getActiveEngine().getHAL().showVirtualKeyboard(false);
}

void LineEdit::onDraw()
{
    using namespace std::chrono;
    if (this != Widget::getCurrentFocus())
        return;

    system_clock::time_point now = system_clock::now();
    if (now >= toggleCursorVisibilityThresholdTime) {
        toggleCursorVisibilityThresholdTime = now + toggleCursorVisiblityPeriod;
        cursor->toggleVisible();
    }
}

void LineEdit::onKeyDown(uint16_t scancode)
{
    chargeBlink();
    switch (scancode) {
    case SCANCODE_LEFT: {
        const auto& old = leftpart->getText();
        if (old.empty())
            break;
        auto s = leftpart->popBack();
        leftpart->minimize();
        rightpart->prepend(s);
        placeWidgets();
        break;
    }
    case SCANCODE_RIGHT: {
        const auto& old = rightpart->getText();
        if (old.empty())
            break;
        auto s = rightpart->popFront();
        rightpart->minimize();
        leftpart->append(s);
        placeWidgets();
        break;
    }
    case SCANCODE_BACKSPACE: {
        const auto& old = leftpart->getText();
        if (old.empty())
            break;
        leftpart->popBack();
        leftpart->minimize();
        placeWidgets();
        text_changed = true;
        invokeAction(onValueChangedActionNames, getText());
        break;
    }
    case SCANCODE_DELETE: {
        const auto& old = rightpart->getText();
        if (old.empty())
            break;
        rightpart->popFront();
        rightpart->minimize();
        placeWidgets();
        text_changed = true;
        invokeAction(onValueChangedActionNames, getText());
        break;
    }
    case SCANCODE_ENTER: {
        auto t = getText();
        onEnterPressedActions(t);
        invokeAction(onEnterActionNames, t);
        break;
    }
    default: return;
    }
}

void LineEdit::onResize(const vvv::vector2i& oldSize,
                        const vvv::vector2i& newSize)
{
    std::ignore = oldSize;
    background->setSize(newSize);
    placeWidgets();
}

void LineEdit::onTextEntered(const std::string& text)
{
    leftpart->append(text);
    placeWidgets();
    text_changed = true;
    invokeAction(onValueChangedActionNames, getText());
}

int LineEdit::roughLeftOffset(int leftMargin, int width, int fullTextWidth,
                              int rightMargin) const
{
    if (fullTextWidth < rightMargin) {
        switch (hAlign) {
        case HALIGN::LEFT: return leftMargin;
        case HALIGN::RIGHT: return rightMargin - fullTextWidth;
        case HALIGN::FILL:
        case HALIGN::CENTER: return (width - fullTextWidth) / 2;
        }
    }

    // Case after setText with long text
    if (rightpart->getText().empty())
        return rightMargin;

    return leftpart->getPosition().x;
}

void LineEdit::placeWidgets()
{
    const auto leftWidth = leftpart->getWidth();
    const auto rightWidth = rightpart->getWidth();
    const auto width = getWidth();

    const auto cursorWidth = cursor->getWidth();
    const auto leftMargin = cursorWidth;
    const auto rightMargin = width - cursorWidth;
    const int fullTextWidth = leftWidth + rightWidth;

    const int roughOffset =
        roughLeftOffset(leftMargin, width, fullTextWidth, rightMargin);

    const int cursorPosition =
        vvv::clamp(leftMargin, rightMargin, roughOffset + leftWidth);
    const int leftOffset = cursorPosition - leftWidth;
    const int textOffsetY = (getHeight() - leftpart->getHeight()) / 2;

    rightpart->setPosition(cursorPosition, textOffsetY);
    leftpart->setPosition(leftOffset, textOffsetY);
    background->setPosition(0, 0);

    cursor->setPosition(cursorPosition, textOffsetY);
}

void LineEdit::lazyUpdateText() const
{
    if (text_changed) {
        text = leftpart->getText() + rightpart->getText();
        text_changed = false;
    }
}

void LineEdit::setOnEnterAction(const std::string& action_name)
{
    onEnterActionNames.push_back(action_name);
}

void LineEdit::setOnValueChangedAction(const std::string& action_name)
{
    onValueChangedActionNames.push_back(action_name);
}

} // namespace vvv3d
