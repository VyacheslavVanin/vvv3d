#ifndef PANEL_H
#define PANEL_H
#include "layout.h"
#include "widget.h"
#include "properties/icolor.h"

namespace vvv3d {

class Panel : public IColorProperty {
public:
    Panel();
    Panel(Layout* layout, Widget* background);
    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;

    void addWidget(Widget* added);
    void removeWidget(Widget* removed);

    void setColor(const Color& color) override;
    const Color& getColor() const override;

    Layout* getLayout();
    Widget* getBackground();
    const Layout* getLayout() const;
    const Widget* getBackground() const;

    // Widget interface
protected:
    void onResize(const vvv::vector2i&, const vvv::vector2i& newSize) override;
    void onPointerMove(int x, int y) override;
    void onButtonPressed(int button, int x, int y) override;
    void onButtonReleased(int button, int x, int y) override;
    void onContentChanged() override;

private:
    Layout* layout;
    Widget* background;

    void rearrange();
};

}

#endif // PANEL_H
