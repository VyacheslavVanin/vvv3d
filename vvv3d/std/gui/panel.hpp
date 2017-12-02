#pragma once
#include "layout.hpp"
#include "properties/icolor.hpp"
#include "properties/icontainer.hpp"
#include "properties/ilayout_props.hpp"
#include "widget.hpp"

namespace vvv3d {

class Panel : public Widget,
              public IColorProperty,
              public IContainer,
              public ILayoutProperties {
public:
    Panel();
    Panel(Layout* layout, Widget* background);
    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;

    void addWidget(Widget* added) override;
    void removeWidget(Widget* removed) override;

    void setColor(const Color& color) override;
    const Color& getColor() const override;

    void setPadding(int padding) override;
    int getPadding() const override;

    void setBorder(int border) override;
    int getBorder() const override;

    void setExpandToContent(bool expand) override;
    bool isExpandToContent() const override;

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

} // namespace vvv3d
