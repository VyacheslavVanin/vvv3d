#pragma once
#include "widget.hpp"
#include <memory>
#include <vvv3d/core/graphics/textures/texture.hpp>

namespace vvv3d {

class ImageWidget final : public Widget {
public:
    explicit ImageWidget();
    explicit ImageWidget(const vvv3d::Texture& texture);
    ~ImageWidget();

    void setTexture(const vvv3d::Texture& texture);

private:
    const vvv3d::Texture* texture = nullptr;

    // Widget interface
protected:
    void onDraw() override;
};

} // namespace vvv3d