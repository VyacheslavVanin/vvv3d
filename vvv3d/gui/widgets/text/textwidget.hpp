#pragma once
#include <vvv3d/gui/widgets/widget.hpp>

#include <memory>
#include <string>

#include <vvv3d/gui/align.hpp>
#include <vvv3d/gui/properties/iautoresize.hpp>
#include <vvv3d/gui/properties/icolor.hpp>
#include <vvv3d/gui/properties/ifont.hpp>
#include <vvv3d/gui/properties/ihalign.hpp>
#include <vvv3d/gui/properties/itext.hpp>
#include <vvv3d/gui/properties/ivalign.hpp>
#include <vvv3d/utils/text.hpp>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {
class Font;

class TextWidget : public Widget,
                   public ITextProperty,
                   public IColorProperty,
                   public IVAlignProperty,
                   public IHAlignProperty,
                   public IAutoResizeProperty,
                   public IFontProperty {
public:
    explicit TextWidget(const std::string& text = "Text");
    ~TextWidget() override;

    void loadResources() override;

    void setText(const std::string& text) override;
    const std::string& getText() const override;
    void setText(std::string&& text);

    void append(const std::string& text);
    void prepend(const std::string& text);

    std::string popBack();
    std::string popFront();

    void setColor(const vvv3d::Color& color) override;
    const vvv3d::Color& getColor() const override;
    void setFont(const vvv3d::Font& font) override;
    const vvv3d::Font& getFont() const override;
    void resizeToContent();

    void setAutoResize(bool value) override;
    bool isAutoResize() const override;

    void setHAlign(HALIGN value) override;
    HALIGN getHAlign() const override;

    void setVAlign(VALIGN value) override;
    VALIGN getVAlign() const override;

private:
    Text text_;
    bool autosize;
    HALIGN halign;
    VALIGN valign;
    vvv3d::Transform transform;

    const std::u32string& getText32() const;
    // Widget interface
    void autoresize();

    int getHAlignOffset() const;
    int getVAlignOffset() const;
    void lazyUpdateText() const;
    void lazyUpdateGeometryData() const;
    vvv3d::Geometry& getGeometry() const;
    int getWidthInPixels() const;

protected:
    void onDraw() override;
};

} // namespace vvv3d
