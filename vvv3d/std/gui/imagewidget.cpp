#include "imagewidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

ImageWidget::ImageWidget() : sprite(), autosize(true) {}

ImageWidget::ImageWidget(const TextureShared& texture) : ImageWidget()
{
    setTexture(texture);
    if (texture) {
        setSize(texture->getWidth(), texture->getHeight());
        autoresize();
    }
}

void ImageWidget::setTexture(const TextureShared& texture)
{
    this->texture = texture;
    sprite.setTexture(*this->texture);
    autoresize();
}

const TextureShared& ImageWidget::getTexture() const { return texture; }

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

bool ImageWidget::isAutoResize() const { return autosize; }

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
    std::ignore = oldSize;
    std::ignore = newSize;
    autoresize();
}

ImageWidget::~ImageWidget() = default;

} // namespace vvv3d
