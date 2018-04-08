#include "imagewidget.hpp"
#include <mutex>
#include <vvv3d/vvv3d.hpp>

namespace vvv3d {

ImageWidget::ImageWidget() {}

ImageWidget::ImageWidget(const Texture& texture) : ImageWidget()
{
    setTexture(texture);
    setSize(texture.getWidth(), texture.getHeight());
}

void ImageWidget::setTexture(const Texture& texture)
{
    sprite.setTexture(texture);
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

ImageWidget::~ImageWidget() = default;

} // namespace vvv3d
