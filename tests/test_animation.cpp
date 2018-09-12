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

TEST_F(vvv3dFixture, Animation)
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
    const auto t_frame = a.duration() / a.size();

    // Frames by time
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(t_frame * 0)));
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(t_frame * 0.5)));
    EXPECT_EQ("texture0"s, tex_names.at(a.getFrame(t_frame * 0.999)));

    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(t_frame * 1.0)));
    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(t_frame * 1.01)));
    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(t_frame * 1.75)));
    EXPECT_EQ("texture1"s, tex_names.at(a.getFrame(t_frame * 1.9999)));

    EXPECT_EQ("texture2"s, tex_names.at(a.getFrame(t_frame * 2.0)));

    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 7.00)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 7.01)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 7.5)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 8)));

    // Overflow - clamp to last frame
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 8.001)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 42)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(t_frame * 100500)));

    // Negative values - get frames in reverse order
    //
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(-t_frame * 0.1)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(-t_frame * 0.9999)));
    EXPECT_EQ("texture7"s, tex_names.at(a.getFrame(-t_frame * 1)));

    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-t_frame * 1.000001)));
    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-t_frame * 1.999)));
    EXPECT_EQ("texture6"s, tex_names.at(a.getFrame(-t_frame * 2.000000)));

    vvv3d::AnimationPlayer player;
    const double start_time = 42;
    player.setAnimation(a, start_time);
    player.setLooped(false);
    auto frame_name = [&player, &tex_names](double t) {
        try {
            return tex_names.at(player.getFrame(t));
        }
        catch (const std::out_of_range& e) {
            std::cerr << e.what() << " on t = " << t << "\n";
            return ""s;
        }
    };

    player.setSpeed(1, start_time + 0.0);
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.0));
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.5));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.5));
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 2.0));
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 2.5));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 3.0));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 3.5));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 7));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 7.5));
    // not looped player - clamps to last frame on overflow
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 42));

    // With speed=2 frames changes 2x faster
    player.setSpeed(2, start_time + 0.0);
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 0.5));
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 1.0));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 1.5));
    EXPECT_EQ("texture4"s, frame_name(start_time + t_frame * 2.0));
    EXPECT_EQ("texture5"s, frame_name(start_time + t_frame * 2.5));
    EXPECT_EQ("texture6"s, frame_name(start_time + t_frame * 3.0));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 3.5));

    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 42.0));

    // Change speed in the middle of animation
    player.setSpeed(1, start_time + 0.0);
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.0));
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.5));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.5));
    player.setSpeed(2, start_time + t_frame * 2.0);
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 2.0));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 2.5));
    EXPECT_EQ("texture4"s, frame_name(start_time + t_frame * 3.0));
    EXPECT_EQ("texture5"s, frame_name(start_time + t_frame * 3.5));
    EXPECT_EQ("texture6"s, frame_name(start_time + t_frame * 4.0));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 4.5));

    // Stop animation then resume
    player.setAnimation(a, start_time);
    player.setSpeed(1, start_time);
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.0));
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.5));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.5));
    player.setSpeed(0, start_time + t_frame * 2.0);
    player.setSpeed(2, start_time + t_frame * 3.0);
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 3.0));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 3.5));
    EXPECT_EQ("texture4"s, frame_name(start_time + t_frame * 4.0));
    EXPECT_EQ("texture5"s, frame_name(start_time + t_frame * 4.5));
    EXPECT_EQ("texture6"s, frame_name(start_time + t_frame * 5.0));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 5.5));

    // Looped animation
    player.setAnimation(a, start_time);
    player.setSpeed(1, start_time);
    player.setLooped(true);
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 0.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 1.0));
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 2.0));
    EXPECT_EQ("texture3"s, frame_name(start_time + t_frame * 3.0));
    EXPECT_EQ("texture4"s, frame_name(start_time + t_frame * 4.0));
    EXPECT_EQ("texture5"s, frame_name(start_time + t_frame * 5.0));
    EXPECT_EQ("texture6"s, frame_name(start_time + t_frame * 6.0));
    EXPECT_EQ("texture7"s, frame_name(start_time + t_frame * 7.0));
    EXPECT_EQ("texture0"s, frame_name(start_time + t_frame * 8.0));
    EXPECT_EQ("texture1"s, frame_name(start_time + t_frame * 9.0));
    EXPECT_EQ("texture2"s, frame_name(start_time + t_frame * 10.0));
}
