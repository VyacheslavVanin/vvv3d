#include "textwidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

TextWidget::TextWidget(const std::string& text)
    : autosize(true), halign(HALIGN::CENTER), valign(VALIGN::CENTER),
      transform(), text_(text)
{
    loadResources();

    resizeToContent();
}

void TextWidget::autoresize()
{
    if (autosize)
        resizeToContent();
}

void TextWidget::setText(const std::string& text)
{
    text_.setText(text);
    autoresize();
}

void TextWidget::setText(std::string&& text)
{
    text_.setText(std::move(text));
    autoresize();
}

const std::string& TextWidget::getText() const { return text_.getText(); }

void TextWidget::append(const std::string& text)
{
    text_.append(text);
    autoresize();
}

void TextWidget::prepend(const std::string& text)
{
    text_.prepend(text);
    autoresize();
}

std::string TextWidget::popBack()
{
    auto ret = text_.popBack();
    autoresize();
    return ret;
}

std::string TextWidget::popFront()
{
    auto ret = text_.popFront();
    autoresize();
    return ret;
}

void TextWidget::setColor(const Color& color) { text_.setColor(color); }

const Color& TextWidget::getColor() const { return text_.getColor(); }

void TextWidget::setFont(const Font& font)
{
    text_.setFont(font);
    autoresize();
}

const Font& TextWidget::getFont() const { return text_.getFont(); }

void TextWidget::resizeToContent()
{
    const int lineWidth = text_.textLineWidth();
    const int lineHeight = text_.textLineHeight();
    setMinSize(lineWidth, lineHeight);
}

void TextWidget::setAutoResize(bool value)
{
    this->autosize = value;
    autoresize();
}

bool TextWidget::isAutoResize() const { return autosize; }

void TextWidget::setHAlign(HALIGN value) { this->halign = value; }
HALIGN TextWidget::getHAlign() const { return halign; }

void TextWidget::setVAlign(VALIGN value) { this->valign = value; }
VALIGN TextWidget::getVAlign() const { return valign; }

int TextWidget::getHAlignOffset() const
{
    const auto& size = getSize();
    switch (halign) {
    case HALIGN::FILL:
    case HALIGN::LEFT: return 0;
    case HALIGN::RIGHT: return size.x - getWidthInPixels();
    case HALIGN::CENTER: return (size.x - getWidthInPixels()) / 2;
    }
    throw std::logic_error("Shouldn't be here");
}

int TextWidget::getVAlignOffset() const
{
    switch (valign) {
    case VALIGN::CENTER: return -(size.y - text_.textLineHeight()) / 2;
    case VALIGN::FILL:
    case VALIGN::TOP: return 0;
    case VALIGN::BOTTOM: return -(size.y - text_.textLineHeight());
    }
    throw std::logic_error("Shouldn't be here");
}

const std::u32string& TextWidget::getText32() const
{
    return text_.getText32();
}

void TextWidget::lazyUpdateText() const { text_.lazyUpdateText(); }

void TextWidget::lazyUpdateGeometryData() const
{
    text_.lazyUpdateGeometryData();
}

Geometry& TextWidget::getGeometry() const { return text_.getGeometry(); }

int TextWidget::getWidthInPixels() const { return text_.getWidthInPixels(); }

void TextWidget::onDraw()
{
    const auto& camera = getCamera();

    auto& transform = this->transform;

    const auto& pos = getAbsolutePosition();

    const auto hAlignOffset = getHAlignOffset();
    const int vAlignOffset = getVAlignOffset();

    const auto posx = pos.x + hAlignOffset;
    const auto posy = -pos.y - text_.getFont().getAscender() + vAlignOffset;
    transform.setPosition(posx, posy, 0);

    text_.draw(camera, transform.getModelMatrix());
}

void TextWidget::loadResources()
{
    text_.loadResources();
}

TextWidget::~TextWidget() = default;

} // namespace vvv3d
