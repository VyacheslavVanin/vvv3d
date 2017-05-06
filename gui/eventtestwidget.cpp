#include "colorrectanglewidget.h"
#include "eventtestwidget.h"
#include "vvv3d/core/graphics/color.h"

EventTestWidget::EventTestWidget()
    : TextWidget("Idle"),
      backgroundWidget(new ColorRectWidget(Color(0.2, 0.6, 0.8, 0.5)))
{
    addChild(backgroundWidget);
    setSize(150, 50);
}

void EventTestWidget::OnPointerEnter(int x, int y)
{
    setText("Enter");
    (void)x;
    (void)y;
}

void EventTestWidget::OnPointerLeave(int x, int y)
{
    setText("Leave");
    (void)x;
    (void)y;
}

void EventTestWidget::OnPointerMove(int x, int y)
{
    setText("Move");
    (void)x;
    (void)y;
}

void EventTestWidget::OnButtonPressed(int button, int x, int y)
{
    setText(std::string("Pressed ") + std::to_string(button));
    (void)x;
    (void)y;
}

void EventTestWidget::OnButtonReleased(int button, int x, int y)
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
