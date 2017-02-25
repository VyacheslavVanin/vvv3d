#include "widget.h"
#include "guilayer.h"
#include <vvv3d/vvvmath/linalg.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <vvv3d/std/draw.h>
#include "rect.h"

class GuiLayer;

Widget* Widget::focus = nullptr;

Widget::Widget() :
    pos(0), size(1),
    minSize(1), maxSize(INT32_MAX), clipArea(0),
    obj(nullptr), parent(nullptr), layer(nullptr),
    children(), hover(false)
{}

void Widget::onDraw()
{}

void Widget::onResize(const vvv::vector2i& oldSize,
                      const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Widget::setGuiLayer(GuiLayer* layer)
{
    this->layer = layer;
    for(auto w: children)
        w->setGuiLayer(layer);
}

void Widget::invokePointerMove(int x, int y)
{
    OnPointerMove(x, y);
    const auto oldHover = hover;
    hover = rectContainPoint(clipArea, x, y);

    if(hover == oldHover)
        return;

    if(hover)
        OnPointerEnter(x, y);
    else
        OnPointerLeave(x, y);
}

void Widget::invokeButtonPressed(int button, int x, int y)
{
    Widget* prevFocus = focus;
    focus = focusable ? this : focus;
    OnButtonPressed(button, x, y);
    if(focus != this)
        return;

    if(prevFocus) {
        prevFocus->OnLoseFocus();
    }
    OnGetFocus();
}

void Widget::invokeButtonReleased(int button, int x, int y)
{
    OnButtonReleased(button, x, y);
}

void Widget::invokeTextEntered(const std::string &text)
{
    OnTextEntered(text);
}

void Widget::invokeKeyDown(uint16_t scancode)
{
    OnKeyDown(scancode);
}

void Widget::invokeKeyUp(uint16_t scancode)
{
    OnKeyUp(scancode);
}

Widget *Widget::getCurrentFocus()
{
    return focus;
}

bool Widget::isFocusable() const
{
    return focusable;
}

void Widget::setFocusable(bool focusable)
{
    this->focusable = focusable;
}

static Rect RectToScissor(const Rect& r, const vvv::vector2i& layerSize)
{
    const vvv::vector2i pos(r.x, r.y);
    const vvv::vector2i size(r.z, r.w);
    return Rect(pos.x, layerSize.y - (pos.y + size.y),
                size.x, size.y);
}

void Widget::updateClipArea()
{
    const auto& absPos = getAbsolutePosition();
    clipArea.set(absPos.x, absPos.y, size.x, size.y);
    if(parent)
        clipArea = rectIntersection(clipArea, parent->clipArea);
}

void Widget::Draw()
{
    updateClipArea();

    const Rect& clip = RectToScissor(clipArea, layer->getSize());
    scissor(clip.x, clip.y, clip.z, clip.w);
    onDraw();
    scissorDisable();
    for(auto c: children)
        c->Draw();
}

const vvv::vector2i& Widget::getPosition() const
{
    return pos;
}

void Widget::setPosition(const vvv::vector2i& newPos)
{
    setPosition(newPos.x, newPos.y);
}

void Widget::setPosition(int x, int y)
{
    pos.set(x, y);
}

const vvv::vector2i& Widget::getSize() const
{
    return size;
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
    using namespace vvv;
    const auto oldSize = getSize();

    size.x = clamp_fast(minSize.x, maxSize.x, width);
    size.y = clamp_fast(minSize.y, maxSize.y, height);

    const auto& newSize = getSize();
    onResize(oldSize, newSize);
}

const vvv::vector2i& Widget::getMinSize() const
{
    return minSize;
}

const vvv::vector2i& Widget::getMaxSize() const
{
    return maxSize;
}

void Widget::setMinSize(int width, int height)
{
    using namespace vvv;
    minSize.set(clamp(1, maxSize.x, width),
                clamp(1, maxSize.y, height));
    /// Update size with new constraint
    const auto currentSize = getSize();
    setSize(currentSize.x, currentSize.y);
}

void Widget::setMinSize(const vvv::vector2i& size)
{
    setMinSize(size.x, size.y);
}

void Widget::setMaxSize(int width, int height)
{
    using namespace vvv;
    maxSize.set(clamp(INT32_MAX, minSize.x, width),
                clamp(INT32_MAX, minSize.y, height));
    /// Update size with new constraint
    const auto currentSize = getSize();
    setSize(currentSize.x, currentSize.y);
}

void Widget::setMaxSize(const vvv::vector2i& size)
{
    setMaxSize(size.x, size.y);
}

const Rect&Widget::getRect() const {return clipArea;}

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

void Widget::setParent(Widget* newParent)
{
    if(newParent == parent)
        return;
    newParent->addChild(this);
}

Widget* Widget::getParent() const
{
    return parent;
}

bool Widget::addChild(Widget* child)
{
    auto p = child->getParent();
    if(p)
        p->removeChild(child);
    child->parent = this;
    child->layer  = this->layer;

    const auto it = std::find(children.begin(), children.end(), child);
    if(it != children.end())
        return false;

    children.push_back(child);
    return true;
}

bool Widget::removeChild(Widget* child)
{
    const auto it = std::find(children.begin(), children.end(), child);
    if(it != children.end())
        children.erase(it);
    Widget* childParent = child->parent;
    if(childParent == this)
        child->parent = nullptr;
    return true;
}

void Widget::OnPointerEnter(int, int)
{

}

void Widget::OnPointerLeave(int, int)
{

}

void Widget::OnPointerMove(int, int)
{

}

void Widget::OnButtonPressed(int, int, int)
{

}

void Widget::OnButtonReleased(int, int, int)
{

}

void Widget::OnGetFocus()
{

}

void Widget::OnLoseFocus()
{

}

void Widget::OnKeyDown(uint16_t scancode)
{

}

void Widget::OnKeyUp(uint16_t scancode)
{

}

void Widget::OnTextEntered(const std::string &text)
{

}

const Camera& Widget::getCamera() const
{
    return layer->getCamera();
}

const std::vector<Widget*>& Widget::getChildren() const
{
    return children;
}

Widget::~Widget()
{
    if(focus == this)
        focus = nullptr;
    for(auto c: children)
        delete c;
}

Widget& Widget::operator=(Widget&&) noexcept = default;
Widget::Widget(Widget&&) noexcept = default;
