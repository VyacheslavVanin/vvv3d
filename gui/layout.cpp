#include "layout.h"
#include <vvv3d/vvvmath/linalg.h>
#include <algorithm>

Layout::Layout() :
    border(1), padding(1)
{  }

void Layout::onResize(const vvv::vector2i& oldSize,
                              const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Layout::OnPointerMove(int x, int y)
{
    const auto& children = getChildren();
    for(auto w: children)
        w->invokePointerMove(x, y);
}

void Layout::OnButtonPressed(int button, int x, int y)
{
    const auto& children = getChildren();
    auto w = std::find_if(children.begin(), children.end(),
                          [&x, &y](auto w){
                            return rectContainPoint(w->getRect(), x, y);
                          });
    if (w == children.end())
        return;
    (*w)->invokeButtonPressed(button, x, y);
}

void Layout::OnButtonReleased(int button, int x, int y)
{
    const auto& children = getChildren();
    for (auto w: children)
        w->invokeButtonReleased(button, x, y);
}

void Layout::onAddWidget(Widget* added)
{
    (void)added;
    rearrange();
}

void Layout::onRemoveWidget(Widget* removed)
{
    (void)removed;
    rearrange();
}

void Layout::onContentChanged(Widget* changed)
{
    (void)changed;
    rearrange();
}

void Layout::addWidget(Widget* widget)
{
    addChild(widget);
    rearrange();
}

void Layout::removeWidget(Widget* widget)
{
    removeChild(widget);
    rearrange();
}

int Layout::getPadding() const
{
    return padding;
}

void Layout::setPadding(int value)
{
    padding = value;
    rearrange();
}

int Layout::getBorder() const
{
    return border;
}

void Layout::setBorder(int value)
{
    border = value;
    rearrange();
}
