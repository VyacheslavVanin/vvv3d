#include "lineedit.h"
#include <vvv3d/vvv3d.h>

LineEdit::LineEdit(const std::string& text)
    : Widget(), hAlign(HALIGN::LEFT)
{
    auto lp  = std::make_unique<TextWidget>(text);
    auto rp  = std::make_unique<TextWidget>("");
    auto cur = std::make_unique<ColorRectWidget>(Color::BLACK);
    auto bg  = std::make_unique<ColorRectWidget>(Color::WHITE);

    leftpart    = lp.release();
    rightpart   = rp.release();
    cursor      = cur.release();
    background  = bg.release();
    leftpart->setAutoSize(true);
    rightpart->setAutoSize(true);
    addChild(background);
    addChild(leftpart);
    addChild(rightpart);
    addChild(cursor);
    setFocusable(true);

    cursor->setSize(3, 14);
    leftpart->setColor(Color::BLACK);
    rightpart->setColor(Color::BLACK);
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

HALIGN LineEdit::getHAlign() const
{
    return this->hAlign;
}

void LineEdit::setBackgroundColor(const Color& color)
{
    background->setColor(color);
}

void LineEdit::setCursorColor(const Color& color)
{
    cursor->setColor(color);
}

void LineEdit::addOnPressEnterAction(
                             const std::function<void (const std::string&)>& f)
{
    onEnterPressedActions.addAction(f);
}

void LineEdit::OnGetFocus()
{}

void LineEdit::OnLoseFocus()
{}

void LineEdit::OnKeyDown(uint16_t scancode)
{
    switch(scancode) {
    case SCANCODE_LEFT: {
            const auto& old = leftpart->getText();
            if(old.empty())
                break;
            char32_t s = leftpart->popBack();
            rightpart->prepend(s);
            placeWidgets();
            break;
        }
    case SCANCODE_RIGHT: {
            const auto& old = rightpart->getText();
            if(old.empty())
                break;
            char32_t s = rightpart->popFront();
            leftpart->append(s);
            placeWidgets();
            break;
        }
    case SCANCODE_BACKSPACE: {
            const auto& old = leftpart->getText();
            if(old.empty())
                break;
            leftpart->popBack();
            placeWidgets();
            break; }
    case SCANCODE_DELETE: {
            const auto& old = rightpart->getText();
            if(old.empty())
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

void LineEdit::onResize(const vvv::vector2i& oldSize, const vvv::vector2i& newSize)
{
    background->setSize(newSize);
    placeWidgets();
}

void LineEdit::OnTextEntered(const std::string& text)
{
    leftpart->append(text);
    placeWidgets();
}

void LineEdit::placeWidgets()
{
    const auto leftWidth  = leftpart->getWidth();
    const auto rightWidth = rightpart->getWidth();
    const auto width      = getWidth();

    const auto cursorWidth = cursor->getWidth();
    const auto leftMargin = cursorWidth;
    const auto rightMargin = width - cursorWidth;
    const int fullTextWidth = leftWidth + rightWidth;

    int leftOffset = leftpart->getPosition().x;

    if(fullTextWidth < rightMargin) {
        switch (hAlign) {
        case HALIGN::LEFT:   leftOffset = leftMargin; break;
        case HALIGN::RIGHT:  leftOffset = rightMargin - fullTextWidth; break;
        case HALIGN::CENTER: leftOffset = (width - fullTextWidth) / 2; break;
        }
    }

    const int cursorPosition = vvv::clamp(leftMargin, rightMargin,
                                          leftOffset + leftWidth);
    leftOffset = cursorPosition - leftWidth;

    const int textOffsetY = (getHeight() - leftpart->getHeight())/2;

    rightpart->setPosition(cursorPosition, textOffsetY);
    leftpart->setPosition(leftOffset, textOffsetY);
    background->setPosition(0, 0);

    cursor->setPosition(cursorPosition, textOffsetY);
}
