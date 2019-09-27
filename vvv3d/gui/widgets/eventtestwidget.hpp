#pragma once
#include "textwidget.hpp"

namespace vvv3d {

/**
 * @brief The EventTestWidget class is class used to demonstrate
 *    base mouse events */
class EventTestWidget : public TextWidget {
public:
    EventTestWidget();
    EventTestWidget(const EventTestWidget&) = delete;
    EventTestWidget& operator=(const EventTestWidget&) = delete;

    // Widget interface
protected:
    void onPointerEnter(int x, int y) override;

    void onPointerLeave(int x, int y) override;

    void onPointerMove(int x, int y) override;

    void onButtonPressed(int button, int x, int y) override;

    void onButtonReleased(int button, int x, int y) override;

private:
    Widget* backgroundWidget = nullptr;

    // Widget interface
protected:
    void onResize(const vvv::vector2i& oldSize,
                  const vvv::vector2i& newSize) override;
};

} // namespace vvv3d
