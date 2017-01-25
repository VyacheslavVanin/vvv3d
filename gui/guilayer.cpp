#include "guilayer.h"
#include "widget.h"
#include <vvv3d/vvv3d.h>
#include <vector>
#include <queue>
#include <algorithm>

using WidgetsContainer = std::vector<Widget*>;

class GuiPointer
{
public:
    GuiPointer(WidgetsContainer& widgets)
        : widgets(widgets), focus(nullptr)
    {}

    void processInput(const Input& input);

    Widget* getWidgetAtPoint(const vvv::vector2i& pos);

    constexpr static uint16_t NUM_MOUSE_BUTTONS = 8;

private:
    WidgetsContainer& widgets;
    Widget* focus;
    bool    mouseButtonsStates[NUM_MOUSE_BUTTONS] {false};
    void detectMouseMove(const Input::Mouse& mouse);
    void detectMouseButtons(const Input::Mouse& mouse);
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
};

GuiLayer::GuiLayerImpl::GuiLayerImpl() : pointer(this->widgets)
{  }

void GuiLayer::GuiLayerImpl::draw()
{
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
    impl->processInputEvents(input);
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

        // Check widget under cursor
        if (underCursorWidget == nullptr)
            continue;

        if (newState)
            underCursorWidget->invokeButtonPressed(i, pos.x, pos.y);
        else
            underCursorWidget->invokeButtonReleased(i, pos.x, pos.y);
    }
}

void GuiPointer::processInput(const Input& input)
{
    const auto& mouse = input.getMouse();

    detectMouseMove(mouse);
    detectMouseButtons(mouse);
}
