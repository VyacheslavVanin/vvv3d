#include "panel.h"
#include "verticallayout.h"

Panel::Panel():
    Panel(new VerticalLayout(), nullptr)
{

}

Panel::Panel(Layout* layout, Widget* background) : Widget(),
    layout(layout), background(background)
{
    if(background)
        addChild(background);
    addChild(layout);
    layout->setPadding(16);
}

void Panel::rearrange()
{
    const auto widgetSize = getSize();
    const auto layoutSize = layout->getSize();
    const auto layoutHeight = layoutSize.y;
    const auto layoutWidth = layoutSize.x;
    layout->setPosition((widgetSize.x - layoutWidth) / 2,
                        (widgetSize.y - layoutHeight) / 2);
}

void Panel::onResize(const vvv::vector2i&, const vvv::vector2i& newSize)
{
    if(background)
        background->setSize(newSize);
    rearrange();
}

void Panel::addWidget(Widget* added)
{
    layout->addWidget(added);
    setMinSize(layout->getSize());
}

void Panel::removeWidget(Widget* removed)
{
    layout->removeWidget(removed);
}
