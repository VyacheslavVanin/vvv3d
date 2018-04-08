#include <gtest/gtest.h>
#include <vvv3d/vvv3d.hpp>
#include <vvv3d/vvv3dgui.hpp>

class TestEngine : public vvv3d::Engine {
public:
    TestEngine(int argc, char** argv, const char* wname = "Vvv3d Hello World");
    // Engine interface
protected:
    void initialSetup() override;
    void onDraw() override;
    void onResize(int x, int y) override;
    int frame_counter = 0;
};
TestEngine::TestEngine(int argc, char** argv, const char* wname)
    : vvv3d::Engine(argc, argv, wname)
{
}

// Override Engine interface
void TestEngine::initialSetup() { frame_counter = 0; }
void TestEngine::onDraw()
{
    if (++frame_counter > 1)
        stop();
}
void TestEngine::onResize(int x, int y) {}

class vvv3dFixture : public ::testing::Test {
public:
    vvv3dFixture()
    {
        static char name[] = "vvv3d";
        static char* argv[]{name};
        e = new TestEngine(1, argv);
    }

    void SetUp() {}
    void TearDown() {}

    ~vvv3dFixture() { delete e; }

    vvv3d::Engine* e;
};

TEST_F(vvv3dFixture, gui)
{
    e->gui().load(R"(
gui
    panel type=panel pos=[30, 30]
        text type=label text="Hello World"
        button type=button text="Button"
        le type=lineedit
        sl type=sliderh
        im type=image image=default size=[64, 64]
    panel2 type=panel pos=[430, 30]
        text2 type=label text="Hello World"
        hl type=hlayout padding=16
            button2 type=button text="Button1"
            button3 type=button text="Button2"
        le2 type=lineedit
        sl2 type=sliderh
        im2 type=image image=default size=[64, 64]
    )");
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
    EXPECT_EQ(panel->getSize(), vvv::vector2i(150, 250));
    EXPECT_EQ(panel->getBorder(), 15);
    EXPECT_EQ(panel->getPadding(), 16);
    EXPECT_EQ(panel->getPosition(), vvv::vector2i(30, 30));
    EXPECT_EQ(text->getSize(), vvv::vector2i(120, 24));
    EXPECT_EQ(button->getSize(), vvv::vector2i(120, 24));
    EXPECT_EQ(le->getSize(), vvv::vector2i(120, 24));
    EXPECT_EQ(sl->getSize(), vvv::vector2i(120, 20));
    EXPECT_EQ(im->getSize(), vvv::vector2i(120, 64));

    // Check panel expands to store new widget
    auto l = new vvv3d::ButtonText("Button2");
    panel->addWidget(l);
    EXPECT_EQ(panel->getSize(), vvv::vector2i(150, 290));

    // Check panel stretch its content
    panel->setWidth(300);
    EXPECT_EQ(panel->getSize(), vvv::vector2i(300, 290));
    EXPECT_EQ(panel->getBorder(), 15);
    EXPECT_EQ(panel->getPadding(), 16);
    EXPECT_EQ(panel->getPosition(), vvv::vector2i(30, 30));
    EXPECT_EQ(text->getSize(), vvv::vector2i(270, 24));
    EXPECT_EQ(button->getSize(), vvv::vector2i(270, 24));
    EXPECT_EQ(le->getSize(), vvv::vector2i(270, 24));
    EXPECT_EQ(sl->getSize(), vvv::vector2i(270, 20));
    EXPECT_EQ(im->getSize(), vvv::vector2i(270, 64));

    // Check nested layout
    auto hl = gui.get("hl");
    auto button2 = gui.get("button2");
    auto button3 = gui.get("button3");
    EXPECT_EQ(hl->getWidth(), 180);
    EXPECT_EQ(hl->getHeight(), 26);
    EXPECT_EQ(button2->getWidth(), 81);
    EXPECT_EQ(button2->getHeight(), 24);
    EXPECT_EQ(button3->getWidth(), 81);
    EXPECT_EQ(button3->getHeight(), 24);
    EXPECT_TRUE(button2->getPosition().x < button3->getPosition().x);
    EXPECT_TRUE(button2->getPosition().y == button3->getPosition().y);
    EXPECT_EQ(button2->getPosition().x, 1);
    EXPECT_EQ(button2->getPosition().y, 1);
    EXPECT_EQ(button3->getPosition().x, 98);
    EXPECT_EQ(button3->getPosition().y, 1);

    auto panel2 = gui.get("panel2");
    panel2->setWidth(400);
    EXPECT_EQ(button2->getWidth(), 176);
    EXPECT_EQ(button2->getHeight(), 24);
    EXPECT_EQ(button3->getWidth(), 176);
    EXPECT_EQ(button3->getHeight(), 24);
    EXPECT_TRUE(button2->getPosition().x < button3->getPosition().x);
    EXPECT_TRUE(button2->getPosition().y == button3->getPosition().y);
    EXPECT_EQ(button2->getPosition().x, 1);
    EXPECT_EQ(button2->getPosition().y, 1);
    EXPECT_EQ(button3->getPosition().x, 193);
    EXPECT_EQ(button3->getPosition().y, 1);

    // Check render without segfaults
    e->run();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
