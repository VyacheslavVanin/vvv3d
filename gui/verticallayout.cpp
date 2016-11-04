#include "verticallayout.h"
#include <vvv3d/vvvmath/linalg.h>
#include <algorithm>
#include <numeric>

VerticalLayout::VerticalLayout(Widget* parent) :
    Widget(parent),
    border(1), padding(1), align(HALIGN::CENTER)
{

}

void VerticalLayout::onResize(const vvv::vector2i& oldSize,
                              const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
    //rearrange();
}


int VerticalLayout::getMaxChildWidth(const std::vector<Widget*>& children)
{
    if(children.empty())
        return 0;

    const auto max = max_element(children.begin(), children.end(),
                                 [](auto w1, auto w2) {
                                    return w1->getSize().x < w2->getSize().x;
                                 });
    return (*max)->getSize().x;
}

int VerticalLayout::getChildrenHeight(const std::vector<Widget*>& children)
{
    return  std::accumulate(children.begin(), children.end(), 0,
                            [](int sum, Widget* w){
                                return sum + w->getSize().y;
                            });
}

void VerticalLayout::rearrange()
{
    const int  borders = 2*getBorder();

    const auto& children = getChildren();

    const auto numPaddings     = children.size() - 1;
    const int  childrenPadding = numPaddings * getPadding();
    const int  childrenHeight  = getChildrenHeight(children);
    const int  contentHeight   = childrenHeight + childrenPadding;
    const int  allHeight       = contentHeight + borders;

    const auto contentWidth    = getMaxChildWidth(children);
    const auto allWidth        = contentWidth + borders;

    setMinSize(allWidth, allHeight);

    int offsetY = getBorder();
    for(auto w: children){
        int offsetX = getBorder();
        switch(align){
            case HALIGN::LEFT: break;
            case HALIGN::CENTER: offsetX += (contentWidth - w->getWidth()) / 2; break;
            case HALIGN::RIGHT: offsetX += contentWidth - w->getWidth(); break;
        }
        w->setPosition(offsetX, offsetY);
        offsetY += w->getHeight() + getPadding();
    }
}

void VerticalLayout::onAddWidget(Widget* added)
{
    (void)added;
    rearrange();
}

void VerticalLayout::onRemoveWidget(Widget* removed)
{
    (void)removed;
    rearrange();
}

void VerticalLayout::onContentChanged(Widget* changed)
{
    (void)changed;
    rearrange();
}

HALIGN VerticalLayout::getAlign() const
{
    return align;
}

void VerticalLayout::setAlign(HALIGN value)
{
    align = value;
}

void VerticalLayout::addWidget(Widget* widget)
{
    addChild(widget);
    rearrange();
}

void VerticalLayout::removeWidget(Widget* widget)
{
    removeChild(widget);
    rearrange();
}

int VerticalLayout::getPadding() const
{
    return padding;
}

void VerticalLayout::setPadding(int value)
{
    padding = value;
    rearrange();
}

int VerticalLayout::getBorder() const
{
    return border;
}

void VerticalLayout::setBorder(int value)
{
    border = value;
    rearrange();
}
