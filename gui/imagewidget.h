#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include "widget.h"
#include <memory>
#include <vvv3d/core/graphics/textures/texture.h>

class ImageWidget final : public Widget {
public:
    explicit ImageWidget();
    explicit ImageWidget(vvv3d::Texture* texture);
    ~ImageWidget();

    void setTexture(vvv3d::Texture* texture);

private:
    vvv3d::Texture* texture = nullptr;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // IMAGEWIDGET_H
