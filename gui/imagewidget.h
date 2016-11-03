#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include "widget.h"
#include <memory>

class Texture;

class ImageWidget final: public Widget
{
public:
    ImageWidget(Widget* parent = nullptr);
    ImageWidget(Texture* texture, Widget* parent = nullptr);
    ~ImageWidget();

    void setTexture(Texture* texture);

private:
    struct ImageWidgetImpl;
    std::unique_ptr<ImageWidgetImpl> pImpl;

    // Widget interface
protected:
    void onDraw() override;
};

#endif // IMAGEWIDGET_H
