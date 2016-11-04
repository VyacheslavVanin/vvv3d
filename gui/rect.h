#ifndef RECT_H
#define RECT_H
#include <vvv3d/vvvmath/linalg.h>

using Rect = vvv::vector4i;

inline int rectTop(const Rect& r)
{
    return r.y;
}

inline int rectBottom(const Rect& r)
{
    return r.y + r.w;
}

inline int rectLeft(const Rect& r)
{
    return r.x;
}

inline int rectRight(const Rect& r)
{
    return r.x + r.z;
}

inline Rect rectIntersection(const Rect& r1, const Rect& r2)
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


#endif // RECT_H
