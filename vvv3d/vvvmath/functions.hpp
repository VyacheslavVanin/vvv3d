#pragma once
#include <cmath>

namespace vvv {
template <typename T>
/**
 * @brief clamp function
 * @param l1 first limit value
 * @param l2 second limit value
 * @param val value to pass
 * @return value if fits limits else
 *  if val greater than both limits then return max(l1, l2)
 *  if val less than both limits then return min(l1, l2) */
T clamp(const T& l1, const T& l2, const T& val)
{
    const T& mx = l1 > l2 ? l1 : l2;
    const T& mn = l1 > l2 ? l2 : l1;
    return std::max(std::min(mx, val), mn);
}

template <typename T>
/**
 * @brief clamp_fast same as clamp but min <= max
 * @param min
 * @param max
 * @param val
 * @return clamped value */
T clamp_fast(const T& min, const T& max, const T& val)
{
    return std::max(std::min(max, val), min);
}

template <typename T>
T lerp(T k, const T& max, const T& min)
{
    return min + k * (max - min);
}

template <typename T>
T deg2radians(T degries)
{
    return degries / 180.0 * M_PI;
}
template <typename T>
T radians2deg(T radians)
{
    return radians / M_PI * 180.0;
}
} // namespace vvv
