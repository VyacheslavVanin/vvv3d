#include "panel.h"
#include "verticallayout.h"

namespace vvv3d {

Panel::Panel() : Panel(new VerticalLayout(), nullptr) {}

Panel::Panel(Layout* layout, Widget* background)
    : Widget(), layout(layout), background(background)
{
    if (background)
        addChild(background);
    addChild(layout);
    layout->setPadding(16);
}

void Panel::rearrange()
{
    const auto widgetSize   = getSize();
    const auto layoutSize   = layout->getSize();
    const auto layoutHeight = layoutSize.y;
    const auto layoutWidth  = layoutSize.x;
    layout->setPosition((widgetSize.x - layoutWidth) / 2,
                        (widgetSize.y - layoutHeight) / 2);
}

void Panel::onResize(const vvv::vector2i&, const vvv::vector2i& newSize)
{
    if (background)
        background->setSize(newSize);
    rearrange();
}

void Panel::OnPointerMove(int x, int y) { layout->invokePointerMove(x, y); }

void Panel::OnButtonPressed(int button, int x, int y)
{
    layout->invokeButtonPressed(button, x, y);
}

void Panel::OnButtonReleased(int button, int x, int y)
{
    layout->invokeButtonReleased(button, x, y);
}

void Panel::OnContentChanged()
{
    const auto& myMinSize = layout->getMinSize();
    const auto& parentMinSize = getMinSize();
    const auto& maxWidth = std::max(myMinSize.x, parentMinSize.x);
    const auto& maxHeight = std::max(myMinSize.y, parentMinSize.y);
    setMinSize(maxWidth, maxHeight);
}

void Panel::addWidget(Widget* added)
{
    layout->addWidget(added);
    setMinSize(layout->getSize());
}

void Panel::removeWidget(Widget* removed) { layout->removeWidget(removed); }

}
