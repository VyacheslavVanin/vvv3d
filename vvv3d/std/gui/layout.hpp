#pragma once
#include "properties/icontainer.hpp"
#include "properties/ilayout_props.hpp"
#include "widget.hpp"

namespace vvv3d {

class Layout : public Widget, public IContainer, public ILayoutProperties {
public:
    Layout();

    int getBorder() const override;
    void setBorder(int value) override;

    void setPadding(int value) override;
    int getPadding() const override;

    void addWidget(Widget* widget) override;
    void removeWidget(Widget* widget) override;

    void setExpandToContent(bool expand) override;
    bool isExpandToContent() const override;

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

} // namespace vvv3d
