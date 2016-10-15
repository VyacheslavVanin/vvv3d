#include "guilayer.h"
#include "guiobject.h"
#include <set>

struct GuiLayer::GuiLayerImpl
{
    std::set<GuiObject*> widgets;

    void draw();
    void addWidget(GuiObject* widget);
    /**
     * @brief Remove widget from Layer.
     *  After this call you responsible for deleting widget.
     * @param widget widget to remove from this container. */
    void removeWidget(GuiObject* widget);

    ~GuiLayerImpl();
};

void GuiLayer::GuiLayerImpl::draw()
{
    for(auto w: widgets)
        w->Draw();
}

void GuiLayer::GuiLayerImpl::addWidget(GuiObject* widget)
{
    widgets.insert(widget);
}

void GuiLayer::GuiLayerImpl::removeWidget(GuiObject* widget)
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

void GuiLayer::addWidget(GuiObject* widget)
{
    impl->addWidget(widget);
}

void GuiLayer::removeWidget(GuiObject* widget)
{
    impl->removeWidget(widget);
}


GuiLayer::~GuiLayer() = default;
GuiLayer::GuiLayer(GuiLayer&&) noexcept = default;
GuiLayer& GuiLayer::operator=(GuiLayer&&) noexcept = default;
