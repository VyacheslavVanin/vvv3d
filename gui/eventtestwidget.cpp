#include "eventtestwidget.h"
#include "colorrectanglewidget.h"
#include "vvv3d/core/graphics/color.h"

EventTestWidget::EventTestWidget() : TextWidget("Idle"),
    backgroundWidget(new ColorRectWidget(Color(0.2, 0.6, 0.8, 0.5)))
{
    addChild(backgroundWidget);
    setSize(150, 50);
}

void EventTestWidget::OnPointerEnter(int x, int y)
{
    setText("Enter");
}

void EventTestWidget::OnPointerLeave(int x, int y)
{
    setText("Leave");
}

void EventTestWidget::OnPointerMove(int x, int y)
{
    setText("Move");
}

void EventTestWidget::OnButtonPressed(int button, int x, int y)
{
    setText(std::string("Pressed ") + std::to_string(button));
}

void EventTestWidget::OnButtonReleased(int button, int x, int y)
{
    setText(std::string("Released ") + std::to_string(button));
}

void EventTestWidget::onResize(const vvv::vector2i& oldSize,
                               const vvv::vector2i& newSize)
{
    if(backgroundWidget==nullptr)
        return;
    backgroundWidget->setSize(newSize);
}
