#include <gtest/gtest.h>
#include <vvv3d/vvv3d.hpp>

GTEST_TEST(vvv3d, hex_color)
{
    const float unit = 1.0 / 0xFF;
    const auto& color = vvv3d::to_color("0xFFEEDDCC");
    const auto& expected = vvv3d::Color(0xFF, 0xEE, 0xDD, 0xCC) * unit;
    ASSERT_EQ(expected, color);
}

GTEST_TEST(vvv3d, float_color)
{
    const auto& color = vvv3d::to_color("0.125");
    const auto& expected = vvv3d::Color(0.125);
    ASSERT_EQ(expected, color);
}

GTEST_TEST(vvv3d, list_color)
{
    const std::vector<std::string> value = {"0.125", "0.5", "42", "9"};
    const auto& color = vvv3d::to_color(value);
    const auto& expected = vvv3d::Color(0.125, 0.5, 42, 9);
    ASSERT_EQ(expected, color);
}

GTEST_TEST(vvv3d, name_color)
{
    const auto& color = vvv3d::to_color("red");
    ASSERT_EQ(vvv3d::RED, color);
}
