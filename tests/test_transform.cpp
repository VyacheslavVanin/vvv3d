#include <gtest/gtest.h>
#include <vvv3d/core/transform.hpp>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvvmath/functions.hpp>

namespace {
#define EXPECT_NEAR_V3(v1, v2)                                                 \
    {                                                                          \
        const auto tolerance = 0.00001;                                        \
        EXPECT_NEAR(v1.x, v2.x, tolerance);                                    \
        EXPECT_NEAR(v1.y, v2.y, tolerance);                                    \
        EXPECT_NEAR(v1.z, v2.z, tolerance);                                    \
    }

} // namespace

GTEST_TEST(vvv3d, transform_direction)
{
    const auto new_front = vvv::vector3f(-1, 1, -1).normalized();
    vvv3d::Transform t;
    t.setDirection(new_front);

    EXPECT_NEAR_V3(new_front, t.getDirection());
}

GTEST_TEST(vvv3d, transform_rotation)
{
    vvv3d::Transform t;

    t.rotate(vvv::deg2radians(90.0), {0, 1, 0});
    EXPECT_NEAR_V3(vvv::vector3f(1, 0, 0), t.getDirection());

    t.rotate(vvv::deg2radians(90.0), vvv::vector3f(0, 0, 1));
    EXPECT_NEAR_V3(vvv::vector3f(0, 1, 0), t.getDirection());
}

GTEST_TEST(vvv3d, transform_up)
{
    const auto new_front = vvv::vector3f(-1, 1, -1).normalized();
    vvv3d::Transform t;
    t.setDirection(new_front);
    EXPECT_NEAR_V3(new_front, t.getDirection());

    t.setUp(0, 0, 1);
    EXPECT_NEAR_V3(new_front, t.getDirection());
}
