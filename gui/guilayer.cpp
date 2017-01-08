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
        : widgets(widgets), focus(nullptr), hover(nullptr)
    {}

    void processInput(const Input& input);

    Widget* getWidgetAtPoint(const vvv::vector2i& pos);

    constexpr static uint16_t NUM_MOUSE_BUTTONS = 8;

private:
    WidgetsContainer& widgets;
    Widget* focus;
    Widget* hover;
    bool    mouseButtonsStates[NUM_MOUSE_BUTTONS] {false};
    void detectHoverEvents(const Input::Mouse& mousePos);
    void detectMouseButtonEvents(const Input::Mouse& mouse);
    void detectMouseMoveEvent(const Input::Mouse& mouse);
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
    // Find leaves
    std::vector<Widget*> leaves;
    std::deque<Widget*> toDiscover(widgets.begin(), widgets.end());

    while(!toDiscover.empty()) {
        auto next = toDiscover.front();
        toDiscover.pop_front();

        if(next->children.empty())
            leaves.push_back(next);
        else {
            for(auto c: next->children)
                toDiscover.push_back(c);
        }
    }

    // Find in reversed direction, to handle one over another widgets.
    // see Panel widget class: Layer widget over background widget, if we search
    // in forward direction then background widget will be found first
    auto w = std::find_if(leaves.rbegin(), leaves.rend(),
                          [&pos](const auto w){
                            return isPointInWidget(w, pos);
                          });
    if(w == leaves.rend())
        return nullptr;
    return *w;
}

void GuiPointer::detectHoverEvents(const Input::Mouse& mouse)
{
    const auto x = mouse.getMouseX();
    const auto y = mouse.getMouseY();
    const auto& mousePos = mouse.getMousePos();
    const auto currentHover = getWidgetAtPoint(mousePos);
    if (currentHover != hover) {
        if (hover)
            hover->PointerLeave(x, y);
        if (currentHover)
            currentHover->PointerEnter(x, y);
    }
    hover = currentHover;
}

void GuiPointer::detectMouseButtonEvents(const Input::Mouse& mouse)
{
    const auto x = mouse.getMouseX();
    const auto y = mouse.getMouseY();

    for (auto i = 0; i < NUM_MOUSE_BUTTONS; ++i){
        const auto currentButtonState = mouse.buttonDown(i);
        const auto oldButtonState = mouseButtonsStates[i];
        // if event occure
        if (currentButtonState != oldButtonState) {
            if (currentButtonState == true){
                // button pressed
                if (hover) {
                    hover->ButtonPressed(i, x, y);
                    focus = hover;
                } else {
                    focus = nullptr; // pressed in without hover
                }
            } else {
                // button released
                if (focus)
                    focus->ButtonReleased(i, x, y);
            }
        }

        mouseButtonsStates[i] = currentButtonState;
    }
}

void GuiPointer::detectMouseMoveEvent(const Input::Mouse& mouse)
{
    if (focus && (mouse.getMouseRelX() || mouse.getMouseRelY())) {
        focus->PointerMove(mouse.getMouseX(), mouse.getMouseY());
    }
}

void GuiPointer::processInput(const Input& input)
{
    const auto& mouse = input.getMouse();

    // Detect hover events and dispatch to widget
    detectHoverEvents(mouse);

    // Detect mouse buttons state changes
    detectMouseButtonEvents(mouse);

    // Detect mouse move event
    detectMouseMoveEvent(mouse);
}
