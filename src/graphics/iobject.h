#ifndef IOBJECT_H
#define IOBJECT_H
#include <vvvmath/linalg.h>
#include <graphics/colour.h>
#include <graphics/textures/texture.h>
#include <graphics/shaders/shader.h>
#include <graphics/geometry.h>

class IObject
{
public:
    virtual ~IObject();
    virtual const vvv::matrix44f& getModelMatrix() const;
    virtual const vvv::matrix33f& getModelRotationMatrix() const;
    virtual const Colour& getColor0() const;
    virtual const Colour& getColor1() const;
    virtual const Colour& getColor2() const;
    virtual const Colour& getColor3() const;
    virtual const Colour& getColor4() const;
    virtual const Colour& getColor5() const;
    virtual const Colour& getColor6() const;
    virtual const Colour& getColor7() const;
    virtual Texture& getTexture0() const;
    virtual Texture& getTexture1() const;
    virtual Texture& getTexture2() const;
    virtual Texture& getTexture3() const;
    virtual Texture& getTexture4() const;
    virtual Texture& getTexture5() const;
    virtual Texture& getTexture6() const;
    virtual Texture& getTexture7() const;
    virtual const vvv::vector4f& getTexturePosition() const;
    virtual std::shared_ptr<Shader> getShader() const;
    virtual const Geometry& getGeometry() const;
};

#endif // IOBJECT_H
