#pragma once
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {
using Rect = vvv::vector4i;

inline int rectTop(const Rect& r) { return r.y; }

inline int rectBottom(const Rect& r) { return r.y + r.w; }

inline int rectLeft(const Rect& r) { return r.x; }

inline int rectRight(const Rect& r) { return r.x + r.z; }

inline Rect rectIntersection(const Rect& r1, const Rect& r2)
{
    using namespace std;
    const auto left = max(rectLeft(r1), rectLeft(r2));
    const auto right = min(rectRight(r1), rectRight(r2));
    const auto top = max(rectTop(r1), rectTop(r2));
    const auto bottom = min(rectBottom(r1), rectBottom(r2));
    const auto width = max(right - left, 0);
    const auto height = max(bottom - top, 0);
    return Rect(left, top, width, height);
}

template <typename T>
inline bool valueInRange(const T& value, const T& left, const T& right)
{
    return value >= left && value <= right;
}

inline bool rectContainPoint(const Rect& r, const vvv::vector2i& point)
{
    return valueInRange(point.x, rectLeft(r), rectRight(r)) &&
           valueInRange(point.y, rectTop(r), rectBottom(r));
}

inline bool rectContainPoint(const Rect& r, int x, int y)
{
    return valueInRange(x, rectLeft(r), rectRight(r)) &&
           valueInRange(y, rectTop(r), rectBottom(r));
}

} // namespace vvv3d
