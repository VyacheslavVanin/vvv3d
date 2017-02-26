#include <vvv3d/vvv3d.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <assert.h>
#include "guilayer.h"
#include "widget.h"
#include "vvvstlhelper.hpp"

using WidgetsContainer = std::vector<Widget*>;

class GuiPointer
{
public:
    GuiPointer(WidgetsContainer& widgets)
        : widgets(widgets)
    {}

    void processInput(const Input& input);

    Widget* getWidgetAtPoint(const vvv::vector2i& pos);

    constexpr static uint16_t NUM_MOUSE_BUTTONS = 8;

private:
    WidgetsContainer& widgets;
    bool    mouseButtonsStates[NUM_MOUSE_BUTTONS] {false};
    void detectMouseMove(const Input::Mouse& mouse);
    void detectMouseButtons(const Input::Mouse& mouse);
    void processKeyboard(const Input::Keyboard& kbd);
};

struct GuiLayer::GuiLayerImpl
{
    WidgetsContainer widgets;
    vvv::vector2<int>    size;
    /**
     * @brief Orthographic camera; top-left corner (0,0)
     */
    Camera camera;
    GuiPointer pointer;
    bool visible;

    GuiLayerImpl();

    /**
     * @brief Draw all widgets. */
    void draw();
    void resize(int width, int height);
    void processInputEvents(const Input& input);

    /**
     * @brief Add widget to layer.
     *  Container become responsible for deleting widget.
     * @param widget widget to add. */
    void addWidget(Widget* widget);

    /**
     * @brief Remove widget from Layer.
     *  After this call you responsible for deleting widget.
     * @param widget widget to remove from this container. */
    void removeWidget(Widget* widget);

    ~GuiLayerImpl();

public:
    bool getVisible() const;
    void setVisible(bool value);
};

bool GuiLayer::GuiLayerImpl::getVisible() const
{
    return visible;
}

void GuiLayer::GuiLayerImpl::setVisible(bool value)
{
    visible = value;
}

GuiLayer::GuiLayerImpl::GuiLayerImpl()
    : widgets(), size(), camera(), pointer(this->widgets), visible(false)
{  }

void GuiLayer::GuiLayerImpl::draw()
{
    if(!visible)
        return;

    DRAW_TRANSPARENT
    for(auto w: widgets)
        w->Draw();
}

void GuiLayer::GuiLayerImpl::resize(int width, int height)
{
    camera.setOrtho(0, width, -height, 0, 1, -1);
    size.set(width, height);
}

void GuiLayer::GuiLayerImpl::addWidget(Widget* widget)
{
    const auto it = std::find(widgets.begin(), widgets.end(), widget);
    if(it == widgets.end())
        widgets.push_back(widget);
}

void GuiLayer::GuiLayerImpl::removeWidget(Widget* widget)
{
    const auto it = std::find(widgets.begin(), widgets.end(), widget);
    if(it != widgets.end())
        widgets.erase(it);
}


void GuiLayer::GuiLayerImpl::processInputEvents(const Input& input)
{
    pointer.processInput(input);
}

GuiLayer::GuiLayerImpl::~GuiLayerImpl()
{
    for(auto w: widgets)
        delete w;
}


GuiLayer::GuiLayer()
    : impl(std::make_unique<GuiLayerImpl>())
{

}

void GuiLayer::draw()
{
    impl->draw();
}

void GuiLayer::addWidget(Widget* widget)
{
    impl->addWidget(widget);
    widget->setGuiLayer(this);
}

void GuiLayer::removeWidget(Widget* widget)
{
    impl->removeWidget(widget);
}

void GuiLayer::resize(int width, int height)
{
    impl->resize(width, height);
}

const vvv::vector2<int>& GuiLayer::getSize() const
{
    return impl->size;
}

const Camera& GuiLayer::getCamera() const
{
    return impl->camera;
}

void GuiLayer::processInputEvents(const Input& input)
{
    const auto& kbd   = input.getKeyboard();
    if(contain(kbd.getEvents(),
               InputEvent{INPUT_EVENT_TYPE::KEY_DOWN, SCANCODE_ESC}))
        setVisible(!getVisible());

    if(!getVisible())
        return;
    impl->processInputEvents(input);
}

void GuiLayer::setVisible(bool visisble)
{
    impl->setVisible(visisble);
}

bool GuiLayer::getVisible() const
{
    return impl->getVisible();
}


GuiLayer::~GuiLayer() = default;
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
Widget* GuiPointer::getWidgetAtPoint(const vvv::vector2i& pos)
{
    auto w = std::find_if(widgets.rbegin(), widgets.rend(),
                          [&pos](const auto w){
                            return isPointInWidget(w, pos);
                          });
    if(w == widgets.rend())
        return nullptr;
    return *w;
}


void GuiPointer::detectMouseMove(const Input::Mouse& mouse)
{
    if (!mouse.isMoved())
        return;
    const auto x = mouse.getMouseX();
    const auto y = mouse.getMouseY();
    for (auto w: widgets)
        w->invokePointerMove(x, y);
}

void GuiPointer::detectMouseButtons(const Input::Mouse& mouse)
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
            underCursorWidget->invokeButtonPressed(i, pos.x, pos.y);

        if (!newState)
            for (auto w: widgets)
                w->invokeButtonReleased(i, pos.x, pos.y);
    }
}

void GuiPointer::processKeyboard(const Input::Keyboard& kbd)
{
    const auto focus = Widget::getCurrentFocus();
    if(!focus)
        return;

    for(const InputEvent& e: kbd.getEvents()) {
        switch (e.type) {
        case INPUT_EVENT_TYPE::KEY_DOWN: focus->invokeKeyDown(e.scancode); break;
        case INPUT_EVENT_TYPE::KEY_UP: focus->invokeKeyUp(e.scancode); break;
        default: assert("Shouldn't be here"); break;
        }
    }

    if(kbd.hasText()) {
        const auto& text = kbd.getText();
        focus->invokeTextEntered(text);
    }
}

void GuiPointer::processInput(const Input& input)
{
    const auto& mouse = input.getMouse();
    const auto& kbd   = input.getKeyboard();

    detectMouseMove(mouse);
    detectMouseButtons(mouse);
    processKeyboard(kbd);
}
