#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H
#include "align.h"
#include "widget.h"
#include "properties/itext.h"
#include "properties/icolor.h"
#include "properties/ivalign.h"
#include "properties/ihalign.h"
#include "properties/iautoresize.h"
#include <memory>
#include <string>
#include <vvv3d/vvv3d.h>
#include <vvv3d/utils/utf8string.hpp>

namespace vvv3d {
class Font;

class TextWidget : public Widget, public ITextProperty, public IColorProperty,
    public IVAlignProperty, public IHAlignProperty, public IAutoResizeProperty {
public:
    explicit TextWidget(const std::string& text = "Text");
    ~TextWidget() override;

    void setText(const std::string& text) override;
    const std::string& getText() const override;
    void setText(std::string&& text);

    void append(const std::string& text);
    void prepend(const std::string& text);

    std::string popBack();
    std::string popFront();

    void setColor(const vvv3d::Color& color) override;
    const vvv3d::Color& getColor() const override;
    void setFont(const vvv3d::Font& font);
    const vvv3d::Font& getFont() const;
    void resizeToContent();

    void setAutoResize(bool value) override;
    bool isAutoResize() const override;

    void setHAlign(HALIGN value) override;
    HALIGN getHAlign() const override;

    void setVAlign(VALIGN value) override;
    VALIGN getVAlign() const override;

private:
    bool autosize;
    HALIGN halign;
    VALIGN valign;
    vvv3d::Transform transform;
    std::string text;
    mutable std::u32string text32;
    std::shared_ptr<vvv3d::Geometry> geometry;
    const vvv3d::Font* font;
    vvv3d::Color color        = vvv3d::WHITE;
    mutable int widthInPixels = 0;
    mutable bool text_changed;
    mutable bool geometry_changed;

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

}

#endif // TEXTWIDGET_H
