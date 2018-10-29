#include "textwidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

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
    const auto& text32 = text_.getText32();
    const auto& font = text_.getFont();
    const int lineWidth = textLineWidth(text32, font);
    const int lineHeight = textLineHeight(font);
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
    case VALIGN::CENTER: return -(size.y - textLineHeight(text_.getFont())) / 2;
    case VALIGN::FILL:
    case VALIGN::TOP: return 0;
    case VALIGN::BOTTOM: return -(size.y - textLineHeight(text_.getFont()));
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



Text::Text(const std::string& text) : text(text) {
    auto& fontMan = getFontManager();
    font = &fontMan.getFont("default");
    geometry = createTextGeometry(*font, text);
}

static std::unique_ptr<vvv3d::Shader> loadTextShader()
{
    static const char* fsh = R"(
    #version 300 es
    precision mediump float;
    uniform sampler2D texture0;
    uniform vec4 color0;

    in  vec2 out_texCoord;
    out vec4 color;


    void main(void)
    {
        color = color0*texture(texture0, out_texCoord);
    }
    )";

    static const char* vsh = R"(
    #version 300 es
    uniform mat4    modelMatrix;
    uniform mat4    viewProjectionMatrix;

    in vec2 va_position;
    in vec2 va_texCoord;
    out highp vec2  out_texCoord;

    void main(void)
    {
        gl_Position  = viewProjectionMatrix*modelMatrix*vec4(va_position, 0 ,1) ;
        out_texCoord = va_texCoord;
    }
    )";
    return vvv3d::Shader::fromStrings("text", vsh, fsh);
}

void Text::loadResources() {
    auto& sm = getShaderManager();
    sm.add("text", loadTextShader);
}

void Text::setText(const std::string& text)
{
    this->text = text;
    text_changed = true;
}

void Text::setText(std::string&& text)
{
    this->text = std::move(text);
    text_changed = true;
}

const std::string& Text::getText() const { return text; }

void Text::append(const std::string& text)
{
    this->text += text;
    text_changed = true;
}
void Text::prepend(const std::string& text)
{
    this->text = text + this->text;
    text_changed = true;
}

std::string Text::popBack()
{
    auto ret = vvv::getLastChar(text);
    text_changed = true;
    return ret;
}
std::string Text::popFront()
{
    auto ret = vvv::getFirstChar(text);
    text_changed = true;
    return ret;
}

void Text::setColor(const vvv3d::Color& color) { this->color = color; }
const vvv3d::Color& Text::getColor() const { return color; }
void Text::setFont(const vvv3d::Font& font)
{
    this->font = &font;
    geometry_changed = true;
}
const vvv3d::Font& Text::getFont() const
{
    // TODO: return "default" if nullptr
    return *font;
}

void Text::lazyUpdateText() const
{
    if (text_changed) {
        text32 = toU32(text);
        text_changed = false;
        geometry_changed = true;
    }
}
void Text::lazyUpdateGeometryData() const
{
    lazyUpdateText();
    if (geometry_changed) {
        updateTextGeometry(geometry, *font, text32);
        widthInPixels = textLineWidth(text32, *font);
        geometry_changed = false;
    }
}
vvv3d::Geometry& Text::getGeometry() const
{
    lazyUpdateGeometryData();
    return *geometry;
}
int Text::getWidthInPixels() const
{
    lazyUpdateGeometryData();
    return widthInPixels;
}

void Text::draw(const vvv3d::Camera& camera, const vvv::matrix44f& model_matrix)
{
    auto& shaderMan = getShaderManager();
    auto& sh = shaderMan.get("text");

    const auto& geometry = getGeometry();
    const auto& font = getFont();
    const auto& texture = font.getTexture();

    drawTexturedColored(camera, sh, geometry, model_matrix, texture, color);
}

void Text::draw(const vvv3d::Camera& camera, int x, int y)
{
    draw(camera, vvv::matrix44f::createTranslateMatrix(x, y, 0));
}

} // namespace vvv3d
