#ifndef COLOR_H
#define COLOR_H
#include <vvv3d/vvvmath/vector4.h>

namespace vvv3d {

class Color
{
public:
    vvv::vector4f vec;
    Color()                                         : vec()       {}
    Color(const Color&) = default;
    explicit Color(float f)                         : vec(f)      {}
    Color(float r, float g, float b, float a = 1)   : vec(r,g,b,a){}
    explicit Color(const vvv::vector4f& v)          : vec(v)      {}
    Color(const vvv::vector3f& v, float a = 1)      : vec(v,a)    {}

    static Color WHITE;
    static Color SILVER;
    static Color GRAY;
    static Color BLACK;
    static Color RED;
    static Color MAROON;
    static Color YELLOW;
    static Color OLIVE;
    static Color LIME;
    static Color GREEN;
    static Color AQUA;
    static Color TEAL;
    static Color BLUE;
    static Color NAVY;
    static Color FUCHSIA;
    static Color PURPLE;
    static Color ORANGE;
};

}

#endif // COLOR_H
