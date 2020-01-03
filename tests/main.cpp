#include <gtest/gtest.h>

#include <vvv3d/core/hal/hal.hpp>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvv3dgui.hpp>

#include "vvv3dfixture.hpp"

TEST_F(vvv3dFixture, gui)
{
    auto& system_fonts = e->getHAL().GetSystemFonts();
    auto desc = system_fonts.getDesc("DejaVu Sans", "Book");
    ASSERT_NE(nullptr, desc);
    system_fonts.setDefaultRegular(desc);
    e->gui().load(R"(
gui
    panel type=panel pos=[30, 30]
        text type=label text="Hello World"
        button type=button text="Button"
        le type=lineedit
        sl type=sliderh
        im type=image image=default
    panel2 type=panel pos=[430, 30]
        text2 type=label text="Hello World"
        hl type=hlayout padding=16
            button2 type=button text="Button1"
            button3 type=button text="Button2"
        le2 type=lineedit
        sl2 type=sliderh
        im2 type=image image=default
    self_descriptive_panel pos=[30, 500]
        named_label
        named_button
    )");

    const auto& font = vvv3d::getFontManager().getFont("default");
    const auto& font_ascender = font.getAscender();
    const auto& font_descender = font.getDescender();
    EXPECT_EQ(font_ascender, 18);
    EXPECT_EQ(font_descender, -5);

    auto& gui = e->gui();
    auto panel = gui.get<vvv3d::Panel>("panel");
    auto text = gui.get("text");
    auto button = gui.get("button");
    auto le = gui.get("le");
    auto sl = gui.get("sl");
    auto im = gui.get("im");
    ASSERT_TRUE(panel != nullptr);
    ASSERT_TRUE(text != nullptr);
    ASSERT_TRUE(button != nullptr);
    ASSERT_TRUE(le != nullptr);
    ASSERT_TRUE(sl != nullptr);
    ASSERT_TRUE(im != nullptr);

    // Check elements has correct sizes
    EXPECT_EQ(panel->getSize(), vvv::vector2i(286, 439));
    EXPECT_EQ(panel->getBorder(), 15);
    EXPECT_EQ(panel->getPadding(), 16);
    EXPECT_EQ(panel->getPosition(), vvv::vector2i(30, 30));
    EXPECT_EQ(text->getSize(), vvv::vector2i(256, 23));
    EXPECT_EQ(button->getSize(), vvv::vector2i(256, 23));
    EXPECT_EQ(le->getSize(), vvv::vector2i(256, 23));
    EXPECT_EQ(sl->getSize(), vvv::vector2i(256, 20));
    EXPECT_EQ(im->getSize(), vvv::vector2i(256, 256));

    // Check panel expands to store new widget
    auto l = new vvv3d::ButtonText("Button2");
    panel->addWidget(l);
    EXPECT_EQ(panel->getSize(), vvv::vector2i(286, 478));

    // Check panel stretch its content
    panel->setWidth(300);
    EXPECT_EQ(panel->getSize(), vvv::vector2i(300, 478));
    EXPECT_EQ(panel->getBorder(), 15);
    EXPECT_EQ(panel->getPadding(), 16);
    EXPECT_EQ(panel->getPosition(), vvv::vector2i(30, 30));
    EXPECT_EQ(text->getSize(), vvv::vector2i(270, 23));
    EXPECT_EQ(button->getSize(), vvv::vector2i(270, 23));
    EXPECT_EQ(le->getSize(), vvv::vector2i(270, 23));
    EXPECT_EQ(sl->getSize(), vvv::vector2i(270, 20));
    EXPECT_EQ(im->getSize(), vvv::vector2i(256, 256));

    // Check nested layout
    auto hl = gui.get("hl");
    auto button2 = gui.get("button2");
    auto button3 = gui.get("button3");
    EXPECT_EQ(hl->getWidth(), 256);
    EXPECT_EQ(hl->getHeight(), 25);
    EXPECT_EQ(button2->getWidth(), 119);
    EXPECT_EQ(button2->getHeight(), 23);
    EXPECT_EQ(button3->getWidth(), 119);
    EXPECT_EQ(button3->getHeight(), 23);
    EXPECT_TRUE(button2->getPosition().x < button3->getPosition().x);
    EXPECT_TRUE(button2->getPosition().y == button3->getPosition().y);
    EXPECT_EQ(button2->getPosition().x, 1);
    EXPECT_EQ(button2->getPosition().y, 1);
    EXPECT_EQ(button3->getPosition().x, 136);
    EXPECT_EQ(button3->getPosition().y, 1);

    auto panel2 = gui.get("panel2");
    panel2->setWidth(400);
    EXPECT_EQ(button2->getWidth(), 176);
    EXPECT_EQ(button2->getHeight(), 23);
    EXPECT_EQ(button3->getWidth(), 176);
    EXPECT_EQ(button3->getHeight(), 23);
    EXPECT_TRUE(button2->getPosition().x < button3->getPosition().x);
    EXPECT_TRUE(button2->getPosition().y == button3->getPosition().y);
    EXPECT_EQ(button2->getPosition().x, 1);
    EXPECT_EQ(button2->getPosition().y, 1);
    EXPECT_EQ(button3->getPosition().x, 193);
    EXPECT_EQ(button3->getPosition().y, 1);

    auto self_descriptive_panel =
        gui.get<vvv3d::Panel>("self_descriptive_panel");
    ASSERT_NE(nullptr, self_descriptive_panel);
    auto named_label = gui.get<vvv3d::TextWidget>("named_label");
    auto named_button = gui.get<vvv3d::ButtonText>("named_button");
    ASSERT_NE(nullptr, named_label);
    ASSERT_NE(nullptr, named_button);
    EXPECT_EQ(named_label->getText(), std::string("named"));
    EXPECT_EQ(named_button->getText(), std::string("named"));

    int counter = 0;
    e->setDrawFunc([this, &counter] {
        if (e->isRunning() && ++counter > 1) {
            e->stop();
        }
    });
    // Check render without segfaults
    e->run();
}

TEST_F(vvv3dFixture, do_not_expand)
{
    e->gui().load(R"(
gui
    panel type=panel pos=[30, 30]
        hl type=hlayout padding=16
            text type=label text="co"
            value type=label text="0"
    )");

    auto& gui = e->gui();
    auto panel = gui.get("panel");

    gui.setText("value", "222222");
    const auto panel_size = panel->getSize();

    int counter = 0;
    e->setDrawFunc([this, &counter, &panel_size] {
        auto& gui = e->gui();
        auto panel = gui.get("panel");

        switch (counter % 2) {
        case 0: gui.setText("value", "1"); break;
        case 1: gui.setText("value", "22"); break;
        }
        EXPECT_LE(panel->getSize().x, panel_size.x);

        if (e->isRunning() && ++counter > 2) {
            e->stop();
        }
    });
    e->run();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
