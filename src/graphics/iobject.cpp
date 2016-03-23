#include "iobject.h"
#include <stdexcept>
using namespace vvv;


static const matrix44f default_matrix44f;
static const matrix33f default_matrix33f;
static const vector3f  default_vector3f;
static const vector4f  default_vector4f;
static const Colour    default_colour = Colour::YELLOW;


IObject::~IObject(){}

const matrix44f& IObject::getModelMatrix() const
{
    return default_matrix44f;
}

const matrix33f& IObject::getModelRotationMatrix() const
{
    return default_matrix33f;
}

const Colour& IObject::getColor0() const
{
    return default_colour;
}

const Colour& IObject::getColor1() const
{
    return default_colour;
}

const Colour& IObject::getColor2() const
{
    return default_colour;
}

const Colour& IObject::getColor3() const
{
    return default_colour;
}

const Colour& IObject::getColor4() const
{
    return default_colour;
}

const Colour& IObject::getColor5() const
{
    return default_colour;
}

const Colour& IObject::getColor6() const
{
    return default_colour;
}

const Colour& IObject::getColor7() const
{
    return default_colour;
}

Texture& IObject::getTexture0() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture1() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture2() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture3() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture4() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture5() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture6() const
{
    throw std::logic_error("Not implemented");
}

Texture& IObject::getTexture7() const
{
    throw std::logic_error("Not implemented");
}

static const vector4f default_texturePosition = vector4f(0.0f,0.0f,1.0f,1.0f);
const vector4f& IObject::getTexturePosition() const
{
    return default_texturePosition;
}

std::shared_ptr<Shader> IObject::getShader() const
{
    throw std::logic_error("Not implemented");
}

const Geometry &IObject::getGeometry() const
{
    throw std::logic_error("Not implemented");
}
