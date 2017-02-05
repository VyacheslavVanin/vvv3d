#include "textwidget.h"
#include <vvv3d/vvv3d.h>
#include <mutex>

int textLineWidth(const std::u32string& text, const Font& f)
{
    int ret = 0;
    for(auto c: text) {
        const auto& g = f.getGlyph(c);
        ret += g.advance;
    }
    return ret;
}

int textLineHeight(const Font& f)
{
    return f.getAscender() - f.getDescender();
}

struct TextWidget::TextWidgetImpl
{
    TextWidgetImpl(const std::string& text)
        : transform(), text(toU32(text)),
          geometry(), font(), changed(true)
    {
        static std::once_flag flag;
        std::call_once(flag, [](){
            auto& e = Engine::getActiveEngine();
            auto& resman = e.getResourceManager();
            auto& shaderMan = resman.getShaderManager();
            shaderMan.add("text", "data/shaders/text.vsh",
                          "data/shaders/text.fsh");

            auto& fontMan = resman.getFontManager();
            fontMan.addFont("default", "data/fonts/DejaVuSans.ttf", 20);
        } );

        auto& e = Engine::getActiveEngine();
        auto& resman = e.getResourceManager();
        auto& fontMan = resman.getFontManager();
        font = fontMan.getFont("default");
        geometry = createTextGeometry(*font, text);
    }

    template<typename T,
             class = std::enable_if_t<std::is_same<std::remove_reference_t<T>,
                                                   std::u32string>::value>
             >
    void setText(T&& text)
    {
        this->text = std::forward<T>(text);
        changed = true;
    }

    void setText(const std::string& text)
    {
        setText(toU32(text));
    }


    void setFont(std::shared_ptr<Font> font)
    {
        this->font = font;
        changed = true;
    }

    void setColor(const Color& color)
    {
        this->color = color;
    }

    void lazyUpdateGeometryData() const
    {
        if(changed){
            updateTextGeometry(geometry, *font, text);
            widthInPixels = textLineWidth(text, *font);
            changed = false;
        }
    }

    Geometry& getGeometry() const
    {
        lazyUpdateGeometryData();
        return *geometry;
    }

    int getWidthInPixels() const
    {
        lazyUpdateGeometryData();
        return widthInPixels;
    }

    Transform                 transform;
    std::u32string            text;
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Font>     font;
    Color                     color = Color::WHITE;
    mutable int               widthInPixels = 0;
    mutable bool changed;
};




TextWidget::TextWidget(const std::string& text)
    : pImpl(std::make_unique<TextWidgetImpl>(text)),
      autosize(false), halign(HALIGN::CENTER), valign(VALIGN::CENTER)
{
    resizeToContent();
    setMinSize(1, getHeight());
}


void TextWidget::autoresize()
{
    if(autosize)
        resizeToContent();
}

void TextWidget::setText(const std::string& text)
{
    pImpl->setText(text);
    autoresize();
}

void TextWidget::setColor(const Color& color)
{
    pImpl->setColor(color);
}

void TextWidget::setFont(std::shared_ptr<Font> font)
{
    pImpl->setFont(font);
    autoresize();
}

void TextWidget::resizeToContent()
{
    const int lineWidth  = textLineWidth(pImpl->text, *pImpl->font);
    const int lineHeight = textLineHeight(*pImpl->font);
    setSize(lineWidth, lineHeight);
}

void TextWidget::setAutoSize(bool value)
{
    this->autosize = value;
    autoresize();
}

void TextWidget::setHAlign(HALIGN value)
{
    this->halign = value;
}

void TextWidget::setVAlign(VALIGN value)
{
    this->valign = value;
}


int TextWidget::getHAlignOffset() const
{
    const auto& size = getSize();
    switch(halign) {
    case HALIGN::LEFT: return 0;
    case HALIGN::RIGHT: return size.x - pImpl->getWidthInPixels();
    case HALIGN::CENTER: return (size.x - pImpl->getWidthInPixels())/2;
    }
    throw std::logic_error("Shouldn't be here");
}

int TextWidget::getVAlignOffset() const
{
    switch(valign){
    case VALIGN::CENTER: return -(size.y - textLineHeight(*pImpl->font))/2;
    case VALIGN::TOP: return 0;
    case VALIGN::BOTTOM:  return -(size.y - textLineHeight(*pImpl->font));
    }
    throw std::logic_error("Shouldn't be here");
}

void TextWidget::onDraw()
{
    const auto& camera = getCamera();
    auto& e = Engine::getActiveEngine();
    auto& resman = e.getResourceManager();
    auto& shaderMan = resman.getShaderManager();
    auto sh = shaderMan.get("text");

    const auto& geometry = pImpl->getGeometry();
    const auto& font     = *pImpl->font;
    const auto& texture  = font.getTexture();
    auto& transform      = pImpl->transform;

    const auto& pos = getAbsolutePosition();

    const auto hAlignOffset = getHAlignOffset();
    const int vAlignOffset  = getVAlignOffset();

    const auto posx = pos.x + hAlignOffset;
    const auto posy = -pos.y - font.getAscender() + vAlignOffset;
    transform.setPosition(posx, posy, 0);

    drawTexturedColored(camera, *sh, geometry,
                        transform, texture,
                        pImpl->color);
}

TextWidget::~TextWidget() = default;
