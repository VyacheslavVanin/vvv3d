#pragma once
#include "widget.hpp"
#include <memory>
#include <vvv3d/std/gui/properties/iimage.hpp>
#include <vvv3d/std/sprites/sprite.hpp>

namespace vvv3d {

class ImageWidget final : public Widget, public IImageProperty {
public:
    explicit ImageWidget();
    explicit ImageWidget(const vvv3d::Texture& texture);
    ~ImageWidget();

    void setTexture(const vvv3d::Texture& texture) override;
    const Texture& getTexture() const override;

private:
    Sprite sprite;

    // Widget interface
protected:
    void onDraw() override;
};

} // namespace vvv3d
