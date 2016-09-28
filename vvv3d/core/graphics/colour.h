#ifndef COLOR_H
#define COLOR_H
#include <vvv3d/vvvmath/vector4.h>

class Colour
{
public:
    vvv::vector4f vec;
    Colour()                                         : vec()       {}
    Colour(const Colour&) = default;
    Colour(float f)                                  : vec(f)      {}
    Colour(float r, float g, float b, float a = 1)   : vec(r,g,b,a){}
    Colour(const vvv::vector4f& v)                   : vec(v)      {}
    Colour(const vvv::vector3f& v, float a = 1)      : vec(v,a)    {}

    static Colour WHITE;
    static Colour SILVER;
    static Colour GRAY;
    static Colour BLACK;
    static Colour RED;
    static Colour MAROON;
    static Colour YELLOW;
    static Colour OLIVE;
    static Colour LIME;
    static Colour GREEN;
    static Colour AQUA;
    static Colour TEAL;
    static Colour BLUE;
    static Colour NAVY;
    static Colour FUCHSIA;
    static Colour PURPLE;
    static Colour ORANGE;
};

#endif // COLOR_H
