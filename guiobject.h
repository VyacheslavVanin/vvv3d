#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <memory>
#include <vvv3d/vvvmath/matrices_types.h>

class GuiObject
{
public:
    GuiObject();
    GuiObject(GuiObject&&) noexcept;
    GuiObject& operator=(GuiObject&&) noexcept;

    virtual ~GuiObject();

    void Draw();

    const vvv::vector2f& getPosition() const;
    void setPosition(const vvv::vector2f& newPos);

    const vvv::vector2f& getSize() const;
    void setSize(const vvv::vector2f& size);

    const vvv::vector2f getAbsolutePosition() const;

    void setParent(GuiObject* parent);
    GuiObject* getParent() const;

protected:
    /**
     * @brief Implement to draw something */
    virtual void onDraw();
    virtual void onResize(const vvv::vector2f& oldSize,
                          const vvv::vector2f& newSize);

private:
    struct GuiObjectObjectImpl;
    std::unique_ptr<GuiObjectObjectImpl> impl;
};

#endif // GUIOBJECT_H
