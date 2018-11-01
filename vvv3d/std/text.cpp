#include "text.hpp"
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/utils/utf8string.hpp>

namespace vvv3d {

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
        widthInPixels = textLineWidth();
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

    TransparentScope ts;
    drawTexturedColored(camera, sh, geometry, model_matrix, texture, color);
}

void Text::draw(const vvv3d::Camera& camera, int x, int y)
{
    draw(camera, vvv::matrix44f::createTranslateMatrix(x, y, 0));
}

void Text::draw(float x, float y, float z)
{
    const auto& e = vvv3d::Engine::getActiveEngine();
    const auto& viewport = e.getViewport();
    vvv3d::Camera camera;
    camera.setOrtho(viewport);

    const float k = z / 2.0f + 1.0f;
    const float zExpanded = vvv::lerp(z, camera.getZNear(), camera.getZFar());
    const auto& pos = vvv::vector3f(
            std::floor(x * viewport.getWidth() * 0.5f),
            std::floor(y * viewport.getHeight() * 0.5f), zExpanded);
    const auto& model_matrix = vvv::matrix44f::createTranslateMatrix(pos);

    draw(camera, model_matrix);
}

int Text::textLineWidth() const
{
    const auto& text = getText32();
    const auto& f = getFont();
    int ret = 0;
    for (auto c : text) {
        const auto& g = f.getGlyph(c);
        ret += g.advance;
    }
    return ret;
}

int Text::textLineHeight() const { const auto& f = getFont(); return f.getAscender() - f.getDescender(); }

}
