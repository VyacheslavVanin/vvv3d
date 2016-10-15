#include "guiobject.h"
#include "guilayer.h"
#include <vvv3d/vvvmath/linalg.h>
#include <memory>
#include <set>

class GuiLayer;
struct Widget::GuiObjectObjectImpl
{
    vvv::vector2f   pos     {0};
    vvv::vector2f   size    {1};
    Widget*      obj     {nullptr};
    Widget*      parent  {nullptr};
    GuiLayer*    layer   {nullptr};
    std::set<Widget*> children;

    GuiObjectObjectImpl(Widget* obj) : obj(obj) {}

    void removeChild(Widget* child){
        children.erase(child);
        Widget* childParent = child->impl->parent;
        if(childParent == obj)
            child->impl->parent = nullptr;
    }

    void addChild(Widget* child){
        auto p = child->getParent();
        if(p)
            p->removeWidget(child);
        child->impl->parent = obj;
        child->impl->layer  = obj->impl->layer;
        children.insert(child);
    }

    void setParent(Widget* newParent)
    {
        if(newParent == parent) return;
        newParent->addWidget(obj);
    }

    Widget* getParent() const {
        return parent;
    }

};

Widget::Widget()
    : impl(std::make_unique<GuiObjectObjectImpl>(this))
{}

void Widget::onDraw()
{}

void Widget::onResize(const vvv::vector2f& oldSize,
                         const vvv::vector2f& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Widget::setGuiLayer(GuiLayer* layer)
{
    impl->layer = layer;
}

void Widget::Draw()
{
    onDraw();
    for(auto c: impl->children)
        c->Draw();
}

const vvv::vector2f& Widget::getPosition() const
{
    return impl->pos;
}

void Widget::setPosition(const vvv::vector2f& newPos)
{
    impl->pos = newPos;
}

const vvv::vector2f& Widget::getSize() const
{
    return impl->size;
}

void Widget::setSize(const vvv::vector2f& size)
{
    const auto oldSize = impl->size;
    impl->size = size;
    onResize(oldSize, size);
}

const vvv::vector2f Widget::getAbsolutePosition() const
{
    auto ret = getPosition();
    auto parent = getParent();
    while(parent){
        ret += parent->getPosition();
        parent = parent->getParent();
    }
    return ret;
}

void Widget::setParent(Widget* parent)
{
    impl->setParent(parent);
}

Widget* Widget::getParent() const
{
    return impl->getParent();
}

void Widget::addWidget(Widget* widget)
{
    impl->addChild(widget);
}

void Widget::removeWidget(Widget* widget)
{
    impl->removeChild(widget);
}

const Camera& Widget::getCamera() const
{
    return impl->layer->getCamera();
}

Widget::~Widget() = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
Widget::Widget(Widget&&) noexcept = default;
