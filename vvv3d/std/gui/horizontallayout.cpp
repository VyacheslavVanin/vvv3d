#include "horizontallayout.hpp"
#include <numeric>

namespace vvv3d {

HorizontalLayout::HorizontalLayout() : align(VALIGN::FILL) {}

void HorizontalLayout::rearrange()
{
    const int borders = 2 * getBorder();
    const auto& children = getChildren();

    const auto numPaddings = children.size() - 1;
    const auto childrenPadding = numPaddings * getPadding();
    const auto childrenWidth = getChildrenWidth(children);
    const auto contentWidth = childrenWidth + childrenPadding;
    const auto allWidth = contentWidth + borders;

    const auto contentHeight = getMaxChildHeight(children);
    const auto allHeight = contentHeight + borders;
    const auto preferedHeight = std::max(contentHeight, allHeight - borders);

    if (isExpandToContent())
        setMinSize(allWidth, allHeight);

    int offsetX = getBorder();
    for (auto w : children) {
        int offsetY = getBorder();

        switch (getVAlign()) {
        case VALIGN::TOP: break;
        case VALIGN::CENTER:
            offsetY += (contentHeight - w->getHeight()) / 2;
            break;
        case VALIGN::BOTTOM: offsetY += contentHeight - w->getHeight(); break;
        case VALIGN::FILL: w->setHeight(preferedHeight); break;
        }
        w->setPosition(offsetX, offsetY);
        offsetX += w->getWidth() + getPadding();
    }
}

int HorizontalLayout::getMaxChildHeight(const std::vector<Widget*>& children)
{
    using namespace std;
    if (children.empty())
        return 0;
    const auto max =
        max_element(children.begin(), children.end(), [](auto w1, auto w2) {
            return w1->getSize().y < w2->getSize().y;
        });
    return (*max)->getSize().y;
}

int HorizontalLayout::getChildrenWidth(const std::vector<Widget*>& children)
{
    return std::accumulate(
        children.begin(), children.end(), 0,
        [](int sum, Widget* w) { return sum + w->getSize().x; });
}

VALIGN HorizontalLayout::getVAlign() const { return align; }

void HorizontalLayout::setVAlign(VALIGN value) { this->align = value; }

} // namespace vvv3d
