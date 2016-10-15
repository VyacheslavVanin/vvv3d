#include "guiobject.h"
#include <vvv3d/vvvmath/linalg.h>
#include <memory>
#include <set>

struct GuiObject::GuiObjectObjectImpl
{
    vvv::vector2f   pos     {0};
    vvv::vector2f   size    {1};
    GuiObject*      obj     {nullptr};
    GuiObject*      parent  {nullptr};
    std::set<GuiObject*> children;

    GuiObjectObjectImpl(GuiObject* obj) : obj(obj) {}

    void removeChild(GuiObject* child){
        children.erase(child);
        GuiObject* childParent = child->impl->parent;
        if(childParent == obj)
            child->impl->parent = nullptr;
    }

    void setParent(GuiObject* newParent)
    {
        if(newParent == parent) return;

        if(parent)
            parent->impl->removeChild(obj);

        parent = newParent;
        parent->impl->children.insert(obj);
    }

    GuiObject* getParent() const {
        return parent;
    }

};

GuiObject::GuiObject()
    : impl(std::make_unique<GuiObjectObjectImpl>(this))
{}

void GuiObject::onDraw()
{}

void GuiObject::onResize(const vvv::vector2f& oldSize,
                         const vvv::vector2f& newSize)
{
    (void)oldSize;
    (void)newSize;
}

void GuiObject::Draw()
{
    onDraw();
    for(auto c: impl->children)
        c->Draw();
}

const vvv::vector2f& GuiObject::getPosition() const
{
    return impl->pos;
}

void GuiObject::setPosition(const vvv::vector2f& newPos)
{
    impl->pos = newPos;
}

const vvv::vector2f& GuiObject::getSize() const
{
    return impl->size;
}

void GuiObject::setSize(const vvv::vector2f& size)
{
    const auto oldSize = impl->size;
    impl->size = size;
    onResize(oldSize, size);
}

const vvv::vector2f GuiObject::getAbsolutePosition() const
{
    auto ret = getPosition();
    auto parent = getParent();
    while(parent){
        ret += parent->getPosition();
        parent = parent->getParent();
    }
    return ret;
}

void GuiObject::setParent(GuiObject* parent)
{
    impl->setParent(parent);
}

GuiObject*GuiObject::getParent() const
{
    return impl->getParent();
}

GuiObject::~GuiObject() = default;
GuiObject& GuiObject::operator=(GuiObject&&) noexcept = default;
GuiObject::GuiObject(GuiObject&&) noexcept = default;
