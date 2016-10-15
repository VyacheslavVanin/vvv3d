#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include "guiobject.h"
#include <memory>

class Texture;

class ImageWidget final: public Widget
{
public:
    ImageWidget();
    ImageWidget(Texture* texture);
    ~ImageWidget();

    void setTexture(Texture* texture);


private:
    struct ImageWidgetImpl;
    std::unique_ptr<ImageWidgetImpl> pImpl;

    // Widget interface
protected:
    void onDraw();
    void onResize(const vvv::vector2f& oldSize, const vvv::vector2f& newSize);
};

#endif // IMAGEWIDGET_H
