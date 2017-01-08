#include "guilayer.h"
#include "widget.h"
#include <vvv3d/vvv3d.h>
#include <vector>
#include <algorithm>

using WidgetsContainer = std::vector<Widget*>;

class GuiPointer
{
public:
    GuiPointer(WidgetsContainer& widgets)
        : widgets(widgets), currentFocus(nullptr)
    {}

    void processInput(const Input& input);

    Widget* getWidgetAtPoint(const vvv::vector2i& pos);

private:
    WidgetsContainer& widgets;
    Widget* currentFocus;
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
    auto w = std::find_if(widgets.begin(), widgets.end(),
                          [&pos](const auto w){
                            return isPointInWidget(w, pos);
                          });
    if(w == widgets.end())
        return nullptr;
    return *w;
}

void GuiPointer::processInput(const Input& input)
{
    const auto& mouse = input.getMouse();
    const auto& pos   = mouse.getMousePos();

    auto newFocus = getWidgetAtPoint(pos);
    auto oldFocus = currentFocus;
    if(newFocus != oldFocus){
        //leaveWidget(oldFocus);
        //enterWidget(newFocus);
         /* Здесь пригодился бы конечный автомат
            а то придется учитывать "признаки":
                - есть/нет фокус
                - зажата ли кнопка, когда мы имеем фокус
         */
    }else{
    }

}
