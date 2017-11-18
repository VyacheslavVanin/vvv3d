#ifndef LAYOUT_H
#define LAYOUT_H
#include "widget.h"
#include "properties/icontainer.h"

namespace vvv3d {

class Layout : public IContainer {
public:
    Layout();

    int getBorder() const;
    void setBorder(int value);

    int getPadding() const;
    void setPadding(int value);

    void addWidget(Widget* widget) override;
    void removeWidget(Widget* widget) override;

    void setExpandToFitContent(bool expand);
    bool isExpandToFitContent() const;

protected:
    // Widget interface
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
    void onPointerMove(int x, int y) override;
    void onButtonPressed(int button, int x, int y) override;
    void onButtonReleased(int button, int x, int y) override;

    void onAddWidget(Widget* added);
    void onRemoveWidget(Widget* removed);
    void onContentChanged() override;

    virtual void rearrange() = 0;

private:
    int border;
    int padding;
    bool expandToFit = true;
};

}

#endif // LAYOUT_H
