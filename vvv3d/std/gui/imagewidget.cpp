#include "imagewidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

ImageWidget::ImageWidget() : sprite(), autosize(true) {}

ImageWidget::ImageWidget(const Texture& texture) : ImageWidget()
{
    setTexture(texture);
    setSize(texture.getWidth(), texture.getHeight());
    autoresize();
}

void ImageWidget::setTexture(const Texture& texture)
{
    sprite.setTexture(texture);
    autoresize();
}

const Texture& ImageWidget::getTexture() const { return sprite.getTexture(); }

void ImageWidget::onDraw()
{
    const auto& camera = getCamera();

    const auto& size = getSize();
    const auto& pos = getAbsolutePosition() + size / 2;
    sprite.transform.setPosition(pos.x, -pos.y, 0);
    sprite.transform.setScale(size.x, size.y, 0);
    drawSprite(camera, sprite);
}

void ImageWidget::setAutoResize(bool value)
{
    autosize = value;
    autoresize();
}

bool ImageWidget::isAutoResize() const
{
    return autosize;
}

void ImageWidget::autoresize()
{
    if (autosize)
        resizeToContent();
}

void ImageWidget::resizeToContent()
{
    const auto& sprite_size = sprite.getTexture().getSize();
    setSize(sprite_size.x, sprite_size.y);
}

void ImageWidget::onResize(const vvv::vector2i& oldSize,
                           const vvv::vector2i& newSize)
{
    autoresize();
}

ImageWidget::~ImageWidget() = default;

} // namespace vvv3d
