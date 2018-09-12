#include "vvv3dfixture.hpp"
#include <gtest/gtest.h>
#include <vvv3d/std/sprites/animation.hpp>
#include <vvv3d/std/sprites/animationplayer.hpp>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvv3dgui.hpp>

namespace {
template <typename T>
std::ostream& operator<<(std::ostream& str, const std::vector<T>& v)
{
    str << "[";
    for (const auto& e : v)
        str << e << ", ";
    str << "]";
    return str;
}

std::unordered_map<const vvv3d::Texture*, std::string> MakeDummyTextures()
{
    std::unordered_map<const vvv3d::Texture*, std::string> ret;

    using namespace std::string_literals;
    auto& tm = vvv3d::getTextureManager();
    for (size_t i = 0; i < 10; ++i) {
        const auto& name = "texture"s + std::to_string(i);
        tm.add(vvv3d::makeDummyTexture(128, 128, 8 + i), name);
        const auto& t = tm.get(name);
        ret[&t] = name;
    }

    return ret;
}
} // namespace

TEST_F(vvv3dFixture, animation)
{
    using namespace std::string_literals;
    const auto& tex_names = MakeDummyTextures();
    e->load(R"(
animations
    animation frames=["texture0",
                      "texture1",
                      "texture2",
                      "texture3",
                      "texture4",
                      "texture5",
                      "texture6",
                      "texture7"] length=4
    )");
    const auto& am = vvv3d::getAnimationManager();
    const auto& a = am.get("animation");

    EXPECT_EQ("animation"s, a.getName());
    EXPECT_EQ(4.0, a.duration());
    EXPECT_EQ(8, a.size());

    // Frames by time
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(0)));
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(0.1)));
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(0.50)));

    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(0.51)));
    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(0.75)));
    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(1)));

    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(3.51)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(3.75)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(4)));

    // Overflow - clamp to last frame
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(4.001)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(4.1)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(4.50)));

    // Negative values - get frames in reverse order
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(-0.1)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(-0.499)));

    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-0.50)));
    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-0.75)));
    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-0.999)));
}
