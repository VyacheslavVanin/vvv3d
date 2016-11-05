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
