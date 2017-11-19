#include "layout.h"
#include "panel.h"
#include <algorithm>
#include <vvv3d/vvvmath/linalg.h>

namespace vvv3d {

Layout::Layout() : border(1), padding(1) {}

void Layout::onResize(const vvv::vector2i& oldSize,
                      const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Layout::onPointerMove(int x, int y)
{
    const auto& children = getChildren();
    for (auto w : children)
        w->notifyPointerMove(x, y);
}

void Layout::onButtonPressed(int button, int x, int y)
{
    const auto& children = getChildren();
    auto w = std::find_if(children.begin(), children.end(), [&x, &y](auto w) {
        return rectContainPoint(w->getRect(), x, y);
    });
    if (w == children.end())
        return;
    (*w)->notifyButtonPressed(button, x, y);
}

void Layout::onButtonReleased(int button, int x, int y)
{
    const auto& children = getChildren();
    for (auto w : children)
        w->notifyButtonReleased(button, x, y);
}

void Layout::onAddWidget(Widget* added)
{
    (void)added;
    notifyContentChanged();
}

void Layout::onRemoveWidget(Widget* removed)
{
    (void)removed;
    notifyContentChanged();
}

void Layout::addWidget(Widget* widget)
{
    if (!widget)
        return;
    addChild(widget);
    notifyContentChanged();
}

void Layout::removeWidget(Widget* widget)
{
    removeChild(widget);
    notifyContentChanged();
}

void Layout::setExpandToContent(bool expand) { this->expandToFit = expand; }

bool Layout::isExpandToContent() const { return expandToFit; }

int Layout::getPadding() const { return padding; }

void Layout::setPadding(int value)
{
    padding = value;
    notifyContentChanged();
}

int Layout::getBorder() const { return border; }

void Layout::setBorder(int value)
{
    border = value;
    notifyContentChanged();
}

void Layout::onContentChanged() { rearrange(); }

} // namespace vvv3d
