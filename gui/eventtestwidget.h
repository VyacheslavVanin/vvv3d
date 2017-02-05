#ifndef EVENTTESTWIDGET_H
#define EVENTTESTWIDGET_H
#include "textwidget.h"

/**
 * @brief The EventTestWidget class is class used to demonstrate
 *    base mouse events */
class EventTestWidget : public TextWidget
{
public:
    EventTestWidget();
    EventTestWidget(const EventTestWidget&) = delete;
    EventTestWidget& operator=(const EventTestWidget&) = delete;

    // Widget interface
protected:
    void OnPointerEnter(int x, int y) override;

    void OnPointerLeave(int x, int y) override;

    void OnPointerMove(int x, int y) override;

    void OnButtonPressed(int button, int x, int y) override;

    void OnButtonReleased(int button, int x, int y) override;

private:
    Widget* backgroundWidget = nullptr;

    // Widget interface
protected:
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
};


#endif // EVENTTESTWIDGET_H
