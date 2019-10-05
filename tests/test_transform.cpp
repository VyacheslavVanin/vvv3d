#include <gtest/gtest.h>
#include <vvv3d/core/transform.hpp>
#include <vvv3d/vvv3d.hpp>

namespace {
void expect_near_eq(const vvv::vector3f& v1, const vvv::vector3f& v2,
                    double tolerance = 0.000001)
{
    EXPECT_NEAR(v1.x, v2.x, tolerance);
    EXPECT_NEAR(v1.y, v2.y, tolerance);
    EXPECT_NEAR(v1.z, v2.z, tolerance);
}
} // namespace

GTEST_TEST(vvv3d, transform)
{
    const auto new_front = vvv::vector3f(-1, 1, -1).normalized();
    vvv3d::Transform t;
    t.setDirection(new_front);

    expect_near_eq(new_front, t.getDirection());
}
