#pragma once
#include "widget.h"
#include <memory>
#include <vvv3d/core/graphics/textures/texture.h>

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

}

