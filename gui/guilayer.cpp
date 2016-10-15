#include "guilayer.h"
#include "guiobject.h"
#include <vvv3d/vvv3d.h>
#include <set>

struct GuiLayer::GuiLayerImpl
{
    std::set<Widget*> widgets;
    /**
     * @brief Orthographic camera; top-left corner (0,0)
     */
    Camera camera;

    GuiLayerImpl();

    /**
     * @brief Draw all widgets. */
    void draw();
    void resize(int width, int height);

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

GuiLayer::GuiLayerImpl::GuiLayerImpl()
{  }

void GuiLayer::GuiLayerImpl::draw()
{
    for(auto w: widgets)
        w->Draw();
}

void GuiLayer::GuiLayerImpl::resize(int width, int height)
{
    camera.setOrtho(0, width, -height, 0, 1, -1);
}

void GuiLayer::GuiLayerImpl::addWidget(Widget* widget)
{
    widgets.insert(widget);
}

void GuiLayer::GuiLayerImpl::removeWidget(Widget* widget)
{
    widgets.erase(widget);
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

const Camera& GuiLayer::getCamera() const
{
    return impl->camera;
}


GuiLayer::~GuiLayer() = default;
GuiLayer::GuiLayer(GuiLayer&&) noexcept = default;
GuiLayer& GuiLayer::operator=(GuiLayer&&) noexcept = default;
