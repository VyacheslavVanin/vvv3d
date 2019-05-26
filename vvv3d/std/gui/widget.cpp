#include "widget.hpp"
#include "gui_layer_base.hpp"
#include "rect.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include <vvv3d/std/draw.hpp>
#include <vvv3d/vvvmath/functions.hpp>

namespace vvv3d {
class GuiLayerBase;

Widget* Widget::focus = nullptr;

Widget::Widget()
    : pos(0), size(1), minSize(1), maxSize(INT32_MAX), clipArea(0),
      obj(nullptr), parent(nullptr), layer(nullptr), children(), hover(false)
{
}

void Widget::onDraw() {}

void Widget::onResize(const vvv::vector2i& oldSize,
                      const vvv::vector2i& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void Widget::setGuiLayer(GuiLayerBase* layer)
{
    this->layer = layer;
    for (auto w : children)
        w->setGuiLayer(layer);
}

void Widget::notifyPointerMove(int x, int y)
{
    onPointerMove(x, y);
    const auto oldHover = hover;
    hover = rectContainPoint(clipArea, x, y);

    if (hover == oldHover)
        return;

    if (hover)
        onPointerEnter(x, y);
    else
        onPointerLeave(x, y);
}

void Widget::notifyButtonPressed(int button, int x, int y)
{
    Widget* prevFocus = focus;
    focus = focusable ? this : focus;
    onButtonPressed(button, x, y);
    if (focus != this)
        return;

    if (prevFocus) {
        prevFocus->onLoseFocus();
    }
    onGetFocus();
}

void Widget::notifyButtonReleased(int button, int x, int y)
{
    onButtonReleased(button, x, y);
}

void Widget::notifyTextEntered(const std::string& text) { onTextEntered(text); }

void Widget::notifyKeyDown(uint16_t scancode) { onKeyDown(scancode); }

void Widget::notifyKeyUp(uint16_t scancode) { onKeyUp(scancode); }

void Widget::notifyContentChanged()
{
    if (ignore_content_changed)
        return;

    onContentChanged();
    if (parent)
        parent->notifyContentChanged();
}

Widget* Widget::getCurrentFocus() { return focus; }

bool Widget::isFocusable() const { return focusable; }

void Widget::setFocusable(bool focusable) { this->focusable = focusable; }

void Widget::setFocus()
{
    if (!isFocusable())
        return;

    auto oldFocus = Widget::getCurrentFocus();
    if (oldFocus)
        oldFocus->onLoseFocus();
    Widget::focus = this;
    onGetFocus();
}

bool Widget::isVisible() const { return visible; }

void Widget::setVisible(bool visible) { this->visible = visible; }

void Widget::toggleVisible() { setVisible(!isVisible()); }

bool Widget::isIgnoreContentChanged() const { return ignore_content_changed; }

void Widget::setIgnoreContentChanged(bool ignore)
{
    ignore_content_changed = ignore;
}

static Rect RectToScissor(const Rect& r, const vvv::vector2i& layerSize)
{
    const vvv::vector2i pos(r.x, r.y);
    const vvv::vector2i size(r.z, r.w);
    return Rect(pos.x, layerSize.y - (pos.y + size.y), size.x, size.y);
}

void Widget::updateClipArea()
{
    const auto& absPos = getAbsolutePosition();
    clipArea.set(absPos.x, absPos.y, size.x, size.y);
    if (parent)
        clipArea = rectIntersection(clipArea, parent->clipArea);
}

void Widget::Draw()
{
    if (!isVisible())
        return;

    updateClipArea();

    const Rect& clip = RectToScissor(clipArea, layer->getSize());
    scissor(clip.x, clip.y, clip.z, clip.w);
    onDraw();
    scissorDisable();
    for (auto c : children)
        c->Draw();
}

const vvv::vector2i& Widget::getPosition() const { return pos; }

void Widget::setPosition(const vvv::vector2i& newPos)
{
    setPosition(newPos.x, newPos.y);
}

void Widget::setPosition(int x, int y) { pos.set(x, y); }

const vvv::vector2i& Widget::getSize() const { return size; }

int Widget::getWidth() const { return getSize().x; }

int Widget::getHeight() const { return getSize().y; }

void Widget::setSize(const vvv::vector2i& size) { setSize(size.x, size.y); }

void Widget::setSize(int width, int height)
{
    using namespace vvv;
    const auto oldSize = getSize();

    size.x = clamp_fast(minSize.x, maxSize.x, width);
    size.y = clamp_fast(minSize.y, maxSize.y, height);

    const auto& newSize = getSize();
    if (oldSize != newSize) {
        onResize(oldSize, newSize);
        if (parent)
            parent->notifyContentChanged();
    }
}

void Widget::setWidth(int width) { setSize(width, getHeight()); }

void Widget::setHeight(int height) { setSize(getWidth(), height); }

int Widget::getMaxWidth() const { return getMaxSize().x; }
int Widget::getMaxHeight() const { return getMinSize().y; };
void Widget::setMaxWidth(int width) { setMaxSize(width, getMaxHeight()); }
void Widget::setMaxHeight(int height) { setMaxSize(getMaxWidth(), height); }
int Widget::getMinWidth() const { return getMinSize().x; }
int Widget::getMinHeight() const { return getMinSize().y; };
void Widget::setMinWidth(int width) { setMinSize(width, getMinHeight()); }
void Widget::setMinHeight(int height) { setMinSize(getMinWidth(), height); }

const vvv::vector2i& Widget::getMinSize() const { return minSize; }

const vvv::vector2i& Widget::getMaxSize() const { return maxSize; }

void Widget::setMinSize(int width, int height)
{
    using namespace vvv;
    minSize.set(clamp(1, maxSize.x, width), clamp(1, maxSize.y, height));
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

void Widget::minimize() { setSize(getMinSize()); }

const Rect& Widget::getRect() const { return clipArea; }

const vvv::vector2i Widget::getAbsolutePosition() const
{
    auto ret = getPosition();
    auto parent = getParent();
    while (parent) {
        ret += parent->getPosition();
        parent = parent->getParent();
    }
    return ret;
}

void Widget::setParent(Widget* newParent)
{
    if (newParent == parent)
        return;
    newParent->addChild(this);
}

Widget* Widget::getParent() const { return parent; }

bool Widget::addChild(Widget* child)
{
    auto p = child->getParent();
    if (p)
        p->removeChild(child);
    child->parent = this;
    child->setGuiLayer(this->layer);

    const auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
        return false;

    children.push_back(child);
    return true;
}

bool Widget::removeChild(Widget* child)
{
    const auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
        children.erase(it);
    Widget* childParent = child->parent;
    if (childParent == this)
        child->parent = nullptr;
    return true;
}

void Widget::onPointerEnter(int, int) {}

void Widget::onPointerLeave(int, int) {}

void Widget::onPointerMove(int, int) {}

void Widget::onButtonPressed(int, int, int) {}

void Widget::onButtonReleased(int, int, int) {}

void Widget::onGetFocus() {}

void Widget::onLoseFocus() {}

void Widget::onKeyDown(uint16_t scancode) { std::ignore = scancode; }

void Widget::onKeyUp(uint16_t scancode) { std::ignore = scancode; }

void Widget::onTextEntered(const std::string& text) { std::ignore = text; }

void Widget::onContentChanged() {}

const Camera& Widget::getCamera() const { return layer->getCamera(); }

const std::vector<Widget*>& Widget::getChildren() const { return children; }

void Widget::loadResources() {}

Widget::~Widget()
{
    if (focus == this)
        focus = nullptr;
    for (auto c : children)
        delete c;
}

} // namespace vvv3d
