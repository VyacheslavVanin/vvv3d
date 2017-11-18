#include "eventtestwidget.h"
#include "colorrectanglewidget.h"
#include "vvv3d/core/graphics/color.h"

namespace vvv3d {

EventTestWidget::EventTestWidget()
    : TextWidget("Idle"),
      backgroundWidget(new ColorRectWidget(Color(0.2, 0.6, 0.8, 0.5)))
{
    addChild(backgroundWidget);
    setSize(150, 50);
}

void EventTestWidget::onPointerEnter(int x, int y)
{
    setText("Enter");
    (void)x;
    (void)y;
}

void EventTestWidget::onPointerLeave(int x, int y)
{
    setText("Leave");
    (void)x;
    (void)y;
}

void EventTestWidget::onPointerMove(int x, int y)
{
    setText("Move");
    (void)x;
    (void)y;
}

void EventTestWidget::onButtonPressed(int button, int x, int y)
{
    setText(std::string("Pressed ") + std::to_string(button));
    (void)x;
    (void)y;
}

void EventTestWidget::onButtonReleased(int button, int x, int y)
{
    setText(std::string("Released ") + std::to_string(button));
    (void)x;
    (void)y;
}

void EventTestWidget::onResize(const vvv::vector2i& oldSize,
                               const vvv::vector2i& newSize)
{
    if (backgroundWidget == nullptr)
        return;
    backgroundWidget->setSize(newSize);
    (void)oldSize;
}

} // namespace vvv3d
