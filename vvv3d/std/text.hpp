#pragma once
#include <string>
#include <memory>
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/core/camera.hpp>

namespace vvv3d {
class Font;
class Geometry;

class Text {
public:
    Text(const std::string& text = "Text");
    void loadResources();
    void setText(const std::string& text);
    const std::string& getText() const;
    void setText(std::string&& text);

    void append(const std::string& text);
    void prepend(const std::string& text);

    std::string popBack();
    std::string popFront();

    void setColor(const vvv3d::Color& color);
    const vvv3d::Color& getColor() const;
    void setFont(const vvv3d::Font& font);
    const vvv3d::Font& getFont() const;

    int textLineWidth() const;
    int textLineHeight() const;


    void lazyUpdateText() const;
    void lazyUpdateGeometryData() const;
    vvv3d::Geometry& getGeometry() const;

    const std::u32string& getText32() const {
        lazyUpdateText();
        return text32;}
    int getWidthInPixels() const;

    void draw(const vvv3d::Camera& camera, const vvv::matrix44f& model_matrix);
    void draw(const vvv3d::Camera& camera, int x, int y);

private:
    std::string text;
    mutable std::u32string text32;
    std::shared_ptr<vvv3d::Geometry> geometry;
    const vvv3d::Font* font = nullptr;
    vvv3d::Color color = vvv3d::WHITE;
    mutable int widthInPixels = 0;
    mutable bool text_changed = true;
    mutable bool geometry_changed = true;
};
}
