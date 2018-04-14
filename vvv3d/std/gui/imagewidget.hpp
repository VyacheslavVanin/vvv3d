#pragma once
#include "widget.hpp"
#include <memory>
#include <vvv3d/std/gui/properties/iautoresize.hpp>
#include <vvv3d/std/gui/properties/iimage.hpp>
#include <vvv3d/std/sprites/sprite.hpp>

namespace vvv3d {

class ImageWidget final : public Widget,
                          public IImageProperty,
                          public IAutoResizeProperty {
public:
    explicit ImageWidget();
    explicit ImageWidget(const vvv3d::Texture& texture);
    ~ImageWidget();

    void setTexture(const vvv3d::Texture& texture) override;
    const Texture& getTexture() const override;

    void setAutoResize(bool value) override;
    bool isAutoResize() const override;

private:
    Sprite sprite;
    bool autosize;
    void autoresize();
    void resizeToContent();

    // Widget interface
protected:
    void onDraw() override;
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
};

} // namespace vvv3d
