#pragma once
#include <memory>
#include <vvv3d/gui/properties/iautoresize.hpp>
#include <vvv3d/gui/properties/iimage.hpp>
#include <vvv3d/gui/widgets/widget.hpp>
#include <vvv3d/sprites/sprite.hpp>

namespace vvv3d {

class ImageWidget final : public Widget,
                          public IImageProperty,
                          public IAutoResizeProperty {
public:
    explicit ImageWidget();
    explicit ImageWidget(const vvv3d::TextureShared& texture);
    ~ImageWidget();

    void setTexture(const vvv3d::TextureShared& texture) override;
    const TextureShared& getTexture() const override;

    void setAutoResize(bool value) override;
    bool isAutoResize() const override;

private:
    TextureShared texture;
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
