#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include "widget.h"
#include <memory>

class Texture;

class ImageWidget final: public Widget
{
public:
    explicit ImageWidget();
    explicit ImageWidget(Texture* texture);
    ~ImageWidget();

    void setTexture(Texture* texture);

private:
    Texture* texture = nullptr;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // IMAGEWIDGET_H
