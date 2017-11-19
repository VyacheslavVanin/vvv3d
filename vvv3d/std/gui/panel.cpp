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
    const auto widgetSize = getSize();
    const auto layoutSize = layout->getSize();
    const auto layoutHeight = layoutSize.y;
    const auto layoutWidth = layoutSize.x;
    layout->setPosition((widgetSize.x - layoutWidth) / 2,
                        (widgetSize.y - layoutHeight) / 2);
}

void Panel::onResize(const vvv::vector2i&, const vvv::vector2i& newSize)
{
    if (background)
        background->setSize(newSize);
    if (layout)
        layout->setSize(newSize);
    rearrange();
}

void Panel::onPointerMove(int x, int y) { layout->notifyPointerMove(x, y); }

void Panel::onButtonPressed(int button, int x, int y)
{
    layout->notifyButtonPressed(button, x, y);
}

void Panel::onButtonReleased(int button, int x, int y)
{
    layout->notifyButtonReleased(button, x, y);
}

void Panel::onContentChanged()
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

Layout* Panel::getLayout() { return layout; }

Widget* Panel::getBackground() { return background; }

const Layout* Panel::getLayout() const { return layout; }

const Widget* Panel::getBackground() const { return background; }

void Panel::setColor(const Color& color)
{
    auto background = dynamic_cast<IColorProperty*>(getBackground());
    if (background)
        background->setColor(color);
}

const Color& Panel::getColor() const
{
    static const Color default_color = WHITE;
    auto background = dynamic_cast<const IColorProperty*>(getBackground());
    if (background)
        return background->getColor();
    else
        return default_color;
}

void Panel::removeWidget(Widget* removed) { layout->removeWidget(removed); }

void Panel::setPadding(int padding) { getLayout()->setPadding(padding); }

int Panel::getPadding() const { return getLayout()->getPadding(); }

void Panel::setBorder(int border) { getLayout()->setBorder(border); }

int Panel::getBorder() const { return getLayout()->getBorder(); }

void Panel::setExpandToContent(bool expand)
{
    getLayout()->setExpandToContent(expand);
}

bool Panel::isExpandToContent() const
{
    return getLayout()->isExpandToContent();
}

} // namespace vvv3d
