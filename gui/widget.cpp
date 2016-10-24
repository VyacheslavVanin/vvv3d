#include "widget.h"
#include "guilayer.h"
#include <vvv3d/vvvmath/linalg.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <vvv3d/std/draw.h>

using Rect = vvv::vector4i;

static int rectTop(const Rect& r)
{
    return r.y;
}

static int rectBottom(const Rect& r)
{
    return r.y + r.w;
}

static int rectLeft(const Rect& r)
{
    return r.x;
}

static int rectRight(const Rect& r)
{
    return r.x + r.z;
}


static Rect rectIntersection(const Rect& r1, const Rect& r2)
{
    using namespace std;
    const auto left  = max(rectLeft(r1), rectLeft(r2));
    const auto right = min(rectRight(r1), rectRight(r2));
    const auto top   = max(rectTop(r1), rectTop(r2));
    const auto bottom = min(rectBottom(r1), rectBottom(r2));
    const auto width = max(right - left, 0);
    const auto height = max(bottom - top, 0);
    return vvv::vector4i(left, top, width, height);
}

class GuiLayer;
struct Widget::WidgetImpl
{
    vvv::vector2f   pos     {0};
    vvv::vector2f   size    {1};
    vvv::vector4<int>    clipArea;
    Widget*      obj     {nullptr};
    Widget*      parent  {nullptr};
    GuiLayer*    layer   {nullptr};
    std::vector<Widget*> children;

    WidgetImpl(Widget* obj) : obj(obj) {}

    void setGuiLayer(GuiLayer* layer)
    {
        this->layer = layer;
        for(auto w: children)
            w->setGuiLayer(layer);
    }

    void updateClipArea()
    {
        const auto& absPos = obj->getAbsolutePosition();
        clipArea.set(absPos.x, absPos.y, size.x, size.y);
        if(parent)
            clipArea = rectIntersection(clipArea, parent->impl->clipArea);
    }

    void removeChild(Widget* child){
        const auto it = std::find(children.begin(), children.end(), child);
        if(it != children.end())
            children.erase(it);
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
        const auto it = std::find(children.begin(), children.end(), child);
        if(it == children.end())
            children.push_back(child);
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
    : impl(std::make_unique<WidgetImpl>(this))
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
    impl->setGuiLayer(layer);
}

static Rect RectToScissor(const Rect& r, const vvv::vector2i& layerSize)
{
    const vvv::vector2i pos(r.x, r.y);
    const vvv::vector2i size(r.z, r.w);
    return Rect(pos.x, layerSize.y - (pos.y + size.y),
                size.x, size.y);
}

void Widget::Draw()
{
    impl->updateClipArea();
    const Rect& clip = RectToScissor(impl->clipArea, impl->layer->getSize());
    scissor(clip.x, clip.y, clip.z, clip.w);
    onDraw();
    scissorDisable();
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

void Widget::setPosition(float x, float y)
{
    impl->pos.set(x,y);
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

void Widget::setSize(float width, float height)
{
    setSize(vvv::vector2f(width, height));
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
