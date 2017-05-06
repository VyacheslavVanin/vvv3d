#include "textwidget.h"
#include <mutex>
#include <vvv3d/vvv3d.h>

using namespace vvv3d;

int textLineWidth(const std::u32string& text, const Font& f)
{
    int ret = 0;
    for (auto c : text) {
        const auto& g = f.getGlyph(c);
        ret += g.advance;
    }
    return ret;
}

int textLineHeight(const Font& f) { return f.getAscender() - f.getDescender(); }

TextWidget::TextWidget(const std::string& text)
    : autosize(false), halign(HALIGN::CENTER), valign(VALIGN::CENTER),
      transform(), text(toU32(text)), geometry(), font(), changed(true)
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        auto& e         = Engine::getActiveEngine();
        auto& resman    = e.getResourceManager();
        auto& shaderMan = resman.getShaderManager();
        shaderMan.add("text", "data/shaders/text.vsh", "data/shaders/text.fsh");

        auto& fontMan = resman.getFontManager();
        fontMan.addFont("default", "data/fonts/DejaVuSans.ttf", 20);
    });

    auto& e       = Engine::getActiveEngine();
    auto& resman  = e.getResourceManager();
    auto& fontMan = resman.getFontManager();
    font          = fontMan.getFont("default");
    geometry      = createTextGeometry(*font, text);

    resizeToContent();
    setMinSize(1, getHeight());
}

void TextWidget::autoresize()
{
    if (autosize)
        resizeToContent();
}

void TextWidget::setText(const std::string& text) { setText(toU32(text)); }

void TextWidget::setText(const std::u32string& text)
{
    this->text = text;
    changed    = true;
    autoresize();
}

void TextWidget::setText(std::u32string&& text)
{
    this->text = std::move(text);
    changed    = true;
    autoresize();
}

const std::u32string& TextWidget::getText() const { return text; }

void TextWidget::append(const std::string& text) { append(toU32(text)); }

void TextWidget::append(const std::u32string& text)
{
    this->text += text;
    changed = true;
    autoresize();
}

void TextWidget::append(char32_t character)
{
    this->text += character;
    changed = true;
    autoresize();
}

void TextWidget::prepend(const std::string& text) { prepend(toU32(text)); }

void TextWidget::prepend(const std::u32string& text)
{
    this->text = text + this->text;
    changed    = true;
    autoresize();
}

void TextWidget::prepend(char32_t character)
{
    text.insert(text.begin(), character);
    changed = true;
    autoresize();
}

char32_t TextWidget::popBack()
{
    auto ret = text.back();
    text.pop_back();
    changed = true;
    autoresize();
    return ret;
}

char32_t TextWidget::popFront()
{
    auto ret = text.front();
    text.erase(text.begin());
    changed = true;
    autoresize();
    return ret;
}

void TextWidget::setColor(const Color& color) { this->color = color; }

void TextWidget::setFont(std::shared_ptr<Font> font)
{
    this->font = font;
    changed    = true;
    autoresize();
}

void TextWidget::resizeToContent()
{
    const int lineWidth  = textLineWidth(text, *font);
    const int lineHeight = textLineHeight(*font);
    setSize(lineWidth, lineHeight);
}

void TextWidget::setAutoSize(bool value)
{
    this->autosize = value;
    autoresize();
}

void TextWidget::setHAlign(HALIGN value) { this->halign = value; }

void TextWidget::setVAlign(VALIGN value) { this->valign = value; }

int TextWidget::getHAlignOffset() const
{
    const auto& size = getSize();
    switch (halign) {
    case HALIGN::LEFT: return 0;
    case HALIGN::RIGHT: return size.x - getWidthInPixels();
    case HALIGN::CENTER: return (size.x - getWidthInPixels()) / 2;
    }
    throw std::logic_error("Shouldn't be here");
}

int TextWidget::getVAlignOffset() const
{
    switch (valign) {
    case VALIGN::CENTER: return -(size.y - textLineHeight(*font)) / 2;
    case VALIGN::TOP: return 0;
    case VALIGN::BOTTOM: return -(size.y - textLineHeight(*font));
    }
    throw std::logic_error("Shouldn't be here");
}

void TextWidget::lazyUpdateGeometryData() const
{
    if (changed) {
        updateTextGeometry(geometry, *font, text);
        widthInPixels = textLineWidth(text, *font);
        changed       = false;
    }
}

Geometry& TextWidget::getGeometry() const
{
    lazyUpdateGeometryData();
    return *geometry;
}

int TextWidget::getWidthInPixels() const
{
    lazyUpdateGeometryData();
    return widthInPixels;
}

void TextWidget::onDraw()
{
    const auto& camera = getCamera();
    auto& e            = Engine::getActiveEngine();
    auto& resman       = e.getResourceManager();
    auto& shaderMan    = resman.getShaderManager();
    auto sh            = shaderMan.get("text");

    const auto& geometry = getGeometry();
    const auto& font     = *this->font;
    const auto& texture  = font.getTexture();
    auto& transform      = this->transform;

    const auto& pos = getAbsolutePosition();

    const auto hAlignOffset = getHAlignOffset();
    const int vAlignOffset  = getVAlignOffset();

    const auto posx = pos.x + hAlignOffset;
    const auto posy = -pos.y - font.getAscender() + vAlignOffset;
    transform.setPosition(posx, posy, 0);

    drawTexturedColored(camera, *sh, geometry, transform, texture, this->color);
}

TextWidget::~TextWidget() = default;
