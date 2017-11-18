#include "guilayer.h"
#include "vvvstlhelper.hpp"
#include "widget.h"
#include <algorithm>
#include <assert.h>
#include <queue>
#include <vector>
#include <vvv3d/vvv3d.h>

namespace vvv3d {

GuiLayer::GuiLayer()
    : widgets(), size(), camera(), pointer(this->widgets), visible(false)
{
}

void GuiLayer::draw()
{
    if (!visible)
        return;

    DRAW_TRANSPARENT
    for (auto w : widgets)
        w->Draw();
}

void GuiLayer::addWidget(Widget* widget)
{
    const auto it = std::find(widgets.begin(), widgets.end(), widget);
    if (it == widgets.end())
        widgets.push_back(widget);
    widget->setGuiLayer(this);
}

void GuiLayer::removeWidget(Widget* widget)
{
    const auto it = std::find(widgets.begin(), widgets.end(), widget);
    if (it != widgets.end())
        widgets.erase(it);
}

void GuiLayer::resize(int width, int height)
{
    camera.setOrtho(0, width, -height, 0, 1, -1);
    size.set(width, height);
}

const vvv::vector2<int>& GuiLayer::getSize() const { return size; }

const Camera& GuiLayer::getCamera() const { return camera; }

void GuiLayer::processInputEvents(const Input& input)
{
    if (!getVisible())
        return;
    pointer.processInput(input);
}

void GuiLayer::setVisible(bool visible) { this->visible = visible; }

bool GuiLayer::getVisible() const { return visible; }

GuiLayer::~GuiLayer()
{
    for (auto w : widgets)
        delete w;
}

GuiLayer::GuiLayer(GuiLayer&&) noexcept = default;
GuiLayer& GuiLayer::operator=(GuiLayer&&) noexcept = default;

static bool isPointInWidget(const Widget* w, const vvv::vector2i& point)
{
    return rectContainPoint(w->getRect(), point);
}

/**
 * @brief Return pointer to widget that contain pos
 * @param pos point to test
 * @return Widget or nullptr if no widgets under point */
Widget* GuiLayer::GuiPointer::getWidgetAtPoint(const vvv::vector2i& pos)
{
    auto w =
        std::find_if(widgets->rbegin(), widgets->rend(),
                     [&pos](const auto w) { return isPointInWidget(w, pos); });
    if (w == widgets->rend())
        return nullptr;
    return *w;
}

void GuiLayer::GuiPointer::detectMouseMove(const Input::Mouse& mouse)
{
    if (!mouse.isMoved())
        return;
    const auto x = mouse.getMouseX();
    const auto y = mouse.getMouseY();
    for (auto w : *widgets)
        w->notifyPointerMove(x, y);
}

void GuiLayer::GuiPointer::detectMouseButtons(const Input::Mouse& mouse)
{
    const auto& pos = mouse.getMousePos();
    Widget* underCursorWidget = getWidgetAtPoint(pos);
    for (uint16_t i = 0; i < NUM_MOUSE_BUTTONS; ++i) {
        const auto oldState = mouseButtonsStates[i];
        const auto newState = mouse.buttonDown(i);
        mouseButtonsStates[i] = newState;

        if (newState == oldState)
            continue;

        if (newState && underCursorWidget)
            underCursorWidget->notifyButtonPressed(i, pos.x, pos.y);

        if (!newState)
            for (auto w : *widgets)
                w->notifyButtonReleased(i, pos.x, pos.y);
    }
}

void GuiLayer::GuiPointer::processKeyboard(const Input::Keyboard& kbd)
{
    const auto foc = Widget::getCurrentFocus();
    if (!foc)
        return;

    for (const InputEvent& e : kbd.getEvents()) {
        switch (e.type) {
        case INPUT_EVENT_TYPE::KEY_DOWN: foc->notifyKeyDown(e.scancode); break;
        case INPUT_EVENT_TYPE::KEY_UP: foc->notifyKeyUp(e.scancode); break;
        default: assert("Shouldn't be here"); break;
        }
    }

    if (kbd.hasText()) {
        const auto& text = kbd.getText();
        foc->notifyTextEntered(text);
    }
}

void GuiLayer::GuiPointer::processInput(const Input& input)
{
    const auto& mouse = input.getMouse();
    const auto& kbd = input.getKeyboard();

    detectMouseMove(mouse);
    detectMouseButtons(mouse);
    processKeyboard(kbd);
}

} // namespace vvv3d
