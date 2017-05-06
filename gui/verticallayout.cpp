#include "verticallayout.h"
#include <numeric>

VerticalLayout::VerticalLayout() : align(HALIGN::CENTER) {}

void VerticalLayout::rearrange()
{
    const int borders = 2 * getBorder();

    const auto& children = getChildren();

    const auto numPaddings    = children.size() - 1;
    const int childrenPadding = numPaddings * getPadding();
    const int childrenHeight  = getChildrenHeight(children);
    const int contentHeight   = childrenHeight + childrenPadding;
    const int allHeight       = contentHeight + borders;

    const auto contentWidth = getMaxChildWidth(children);
    const auto allWidth     = contentWidth + borders;

    if (isExpandToFitContent())
        setMinSize(allWidth, allHeight);

    int offsetY = getBorder();
    for (auto w : children) {
        int offsetX = getBorder();
        switch (getAlign()) {
        case HALIGN::LEFT: break;
        case HALIGN::CENTER:
            offsetX += (contentWidth - w->getWidth()) / 2;
            break;
        case HALIGN::RIGHT: offsetX += contentWidth - w->getWidth(); break;
        }
        w->setPosition(offsetX, offsetY);
        offsetY += w->getHeight() + getPadding();
    }
}

int VerticalLayout::getMaxChildWidth(const std::vector<Widget*>& children)
{
    if (children.empty())
        return 0;

    const auto max = max_element(children.begin(), children.end(),
                                 [](auto w1, auto w2) {
                                     return w1->getSize().x < w2->getSize().x;
                                 });
    return (*max)->getSize().x;
}

int VerticalLayout::getChildrenHeight(const std::vector<Widget*>& children)
{
    return std::accumulate(
        children.begin(), children.end(), 0,
        [](int sum, Widget* w) { return sum + w->getSize().y; });
}

HALIGN VerticalLayout::getAlign() const { return align; }

void VerticalLayout::setAlign(HALIGN value) { align = value; }
