#include "lineedit.h"
#include <vvv3d/vvv3d.h>

namespace vvv3d {

void LineEdit::adjustCursorHeight()
{
    const auto& font = leftpart->getFont();
    const auto fontHeight = font.getAscender() - font.getDescender();
    cursor->setSize(3, fontHeight);
}

LineEdit::LineEdit(const std::string& text) : Widget(), hAlign(HALIGN::LEFT),
    toggleCursorVisibilityThresholdTime(),
    toggleCursorVisiblityPeriod(std::chrono::milliseconds(750))
{
    auto lp  = std::make_unique<TextWidget>(text);
    auto rp  = std::make_unique<TextWidget>("");
    auto cur = std::make_unique<ColorRectWidget>(Color::BLACK);
    auto bg  = std::make_unique<ColorRectWidget>(Color::WHITE);

    leftpart   = lp.release();
    rightpart  = rp.release();
    cursor     = cur.release();
    background = bg.release();
    leftpart->setAutoSize(true);
    rightpart->setAutoSize(true);
    addChild(background);
    addChild(leftpart);
    addChild(rightpart);
    addChild(cursor);
    setFocusable(true);

    adjustCursorHeight();
    leftpart->setColor(Color::BLACK);
    rightpart->setColor(Color::BLACK);
    cursor->setVisible(false);
}

void LineEdit::setText(const std::string& text)
{
    leftpart->setText(text);
    rightpart->setText("");
}

std::string LineEdit::getText() const
{
    return toU8(leftpart->getText() + rightpart->getText());
}

void LineEdit::setHAlign(HALIGN align)
{
    this->hAlign = align;
    placeWidgets();
}

HALIGN LineEdit::getHAlign() const { return this->hAlign; }

void LineEdit::setBackgroundColor(const Color& color)
{
    background->setColor(color);
}

void LineEdit::setCursorColor(const Color& color) { cursor->setColor(color); }

void LineEdit::addOnPressEnterAction(
    const std::function<void(const std::string&)>& f)
{
    onEnterPressedActions.addAction(f);
}

void LineEdit::chargeBlink()
{
    using namespace std::chrono;
    toggleCursorVisibilityThresholdTime = system_clock::now() +
                                          toggleCursorVisiblityPeriod;
    cursor->setVisible(true);
}

void LineEdit::onGetFocus()
{
    chargeBlink();
}

void LineEdit::onLoseFocus()
{
    using namespace std::chrono;
    toggleCursorVisibilityThresholdTime = system_clock::time_point();
    cursor->setVisible(false);
}

void LineEdit::onDraw()
{
    using namespace std::chrono;
    if (this != Widget::getCurrentFocus())
        return;

    system_clock::time_point now = system_clock::now();
    if (now >= toggleCursorVisibilityThresholdTime) {
        toggleCursorVisibilityThresholdTime = now + toggleCursorVisiblityPeriod;
        cursor->toggleVisibility();
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
        char32_t s = leftpart->popBack();
        rightpart->prepend(s);
        placeWidgets();
        break;
    }
    case SCANCODE_RIGHT: {
        const auto& old = rightpart->getText();
        if (old.empty())
            break;
        char32_t s = rightpart->popFront();
        leftpart->append(s);
        placeWidgets();
        break;
    }
    case SCANCODE_BACKSPACE: {
        const auto& old = leftpart->getText();
        if (old.empty())
            break;
        leftpart->popBack();
        placeWidgets();
        break;
    }
    case SCANCODE_DELETE: {
        const auto& old = rightpart->getText();
        if (old.empty())
            break;
        rightpart->popFront();
        placeWidgets();
        break;
    }
    case SCANCODE_ENTER: {
        auto t = getText();
        onEnterPressedActions.invoke(t);
        break;
    }
    }
}

void LineEdit::onResize(const vvv::vector2i& oldSize,
                        const vvv::vector2i& newSize)
{
    background->setSize(newSize);
    placeWidgets();
}

void LineEdit::onTextEntered(const std::string& text)
{
    leftpart->append(text);
    placeWidgets();
}

int LineEdit::roughLeftOffset(int leftMargin, int width, int fullTextWidth,
                              int rightMargin) const
{
    if (fullTextWidth < rightMargin) {
        switch (hAlign) {
        case HALIGN::LEFT: return leftMargin;
        case HALIGN::RIGHT: return rightMargin - fullTextWidth;
        case HALIGN::CENTER: return (width - fullTextWidth) / 2;
        }
    }

    return leftpart->getPosition().x;
}

void LineEdit::placeWidgets()
{
    const auto leftWidth  = leftpart->getWidth();
    const auto rightWidth = rightpart->getWidth();
    const auto width      = getWidth();

    const auto cursorWidth  = cursor->getWidth();
    const auto leftMargin   = cursorWidth;
    const auto rightMargin  = width - cursorWidth;
    const int fullTextWidth = leftWidth + rightWidth;

    const int roughOffset =
        roughLeftOffset(leftMargin, width, fullTextWidth, rightMargin);

    const int cursorPosition =
        vvv::clamp(leftMargin, rightMargin, roughOffset + leftWidth);
    const int leftOffset  = cursorPosition - leftWidth;
    const int textOffsetY = (getHeight() - leftpart->getHeight()) / 2;

    rightpart->setPosition(cursorPosition, textOffsetY);
    leftpart->setPosition(leftOffset, textOffsetY);
    background->setPosition(0, 0);

    cursor->setPosition(cursorPosition, textOffsetY);
}

}
