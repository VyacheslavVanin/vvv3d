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
    return Rect(left, top, width, height);
}

class GuiLayer;
struct Widget::WidgetImpl
{
    vvv::vector2i   pos     {0};
    vvv::vector2i   size    {1};
    vvv::vector2i   minSize {1};
    vvv::vector2i   maxSize {INT32_MAX};

    Rect            clipArea;
    Widget*      obj     {nullptr};
    Widget*      parent  {nullptr};
    GuiLayer*    layer   {nullptr};
    std::vector<Widget*> children;

    WidgetImpl(Widget* obj) : obj(obj)
    {
    }

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

    const vvv::vector2i& getMinSize() const {return minSize;}
    const vvv::vector2i& getMaxSize() const {return minSize;}

    const vvv::vector2i& getSize() const {return size;}
    void setSize(int width, int height)
    {
        using namespace vvv;
        size.x = clamp_fast(minSize.x, maxSize.x, width);
        size.y = clamp_fast(minSize.y, maxSize.y, height);
    }

    void setMinSize(int width, int height)
    {
        using namespace vvv;
        minSize.set(clamp(1, maxSize.x, width),
                    clamp(1, maxSize.y, height));
        /// Update size with new constraint
        const auto currentSize = getSize();
        setSize(currentSize.x, currentSize.y);
    }

    void setMaxSize(int width, int height)
    {
        using namespace vvv;
        maxSize.set(clamp(INT32_MAX, minSize.x, width),
                    clamp(INT32_MAX, minSize.y, height));
        /// Update size with new constraint
        const auto currentSize = getSize();
        setSize(currentSize.x, currentSize.y);
    }
};

Widget::Widget(Widget* parent)
    : impl(std::make_unique<WidgetImpl>(this))
{
    if(parent)
        parent->addWidget(this);
}

void Widget::onDraw()
{}

void Widget::onResize(const vvv::vector2i& oldSize,
                      const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Widget::onAddWidget(Widget* added)
{
    (void)added;
}

void Widget::onRemoveWidget(Widget* removed)
{
    (void)removed;
}

void Widget::onContentChanged(Widget* changed)
{

}

void Widget::setSizeNoNotify(int width, int height)
{
    const auto oldSize = impl->getSize();
    impl->setSize(width, height);
    const auto& newSize = getSize();
    onResize(oldSize, newSize);
}

void Widget::setPositionNoNotify(int x, int y)
{
    impl->pos.set(x, y);
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

const vvv::vector2i& Widget::getPosition() const
{
    return impl->pos;
}

void Widget::setPosition(const vvv::vector2i& newPos)
{
    setPosition(newPos.x, newPos.y);
}

void Widget::setPosition(int x, int y)
{
    setPositionNoNotify(x, y);
    Widget* parent = getParent();
    if(parent)
        parent->onContentChanged(this);
}

const vvv::vector2i& Widget::getSize() const
{
    return impl->getSize();
}

int Widget::getWidth() const
{
    return getSize().x;
}

int Widget::getHeight() const
{
    return getSize().y;
}

void Widget::setSize(const vvv::vector2i& size)
{
    setSize(size.x, size.y);
}

void Widget::setSize(int width, int height)
{
    setSizeNoNotify(width, height);
    Widget* parent = getParent();
    if(parent)
        parent->onContentChanged(this);
}

const vvv::vector2i& Widget::getMinSize() const
{
    return impl->getMinSize();
}

const vvv::vector2i& Widget::getMaxSize() const
{
    return impl->getMaxSize();
}

void Widget::setMinSize(int width, int height)
{
    impl->setMinSize(width, height);
}

void Widget::setMinSize(const vvv::vector2i& size)
{
    impl->setMinSize(size.x, size.y);
}

void Widget::setMaxSize(int width, int height)
{
    impl->setMaxSize(width, height);
}

void Widget::setMaxSize(const vvv::vector2i& size)
{
    impl->setMaxSize(size.x, size.y);
}

const vvv::vector2i Widget::getAbsolutePosition() const
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
    onAddWidget(widget);
}

void Widget::removeWidget(Widget* widget)
{
    impl->removeChild(widget);
    onRemoveWidget(widget);
}

const Camera& Widget::getCamera() const
{
    return impl->layer->getCamera();
}

const std::vector<Widget*>&Widget::getChildren() const
{
    return impl->children;
}

Widget::~Widget() = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
Widget::Widget(Widget&&) noexcept = default;
