#include "sprites/sprite.h"
#include <gui/buttonbase.h>
#include <gui/buttontext.h>
#include <gui/colorrectanglewidget.h>
#include <gui/guilayer.h>
#include <gui/horizontallayout.h>
#include <gui/imagewidget.h>
#include <gui/lineedit.h>
#include <gui/panel.h>
#include <gui/textwidget.h>
#include <gui/verticallayout.h>
#include <iomanip>
#include <memory>
#include <random>
#include <string>
#include <vvv3d/vvv3d.h>
#include <vvvstlhelper.hpp>

using namespace vvv3d;

vvv::vector3f randomVector(float range)
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-range, range);
    return {uniform_dist(e1), uniform_dist(e1), 0};
}

class TestEngine : public Engine {
public:
    TestEngine(int argc, char** argv, const char* wname = "TextEngine");
    // Engine interface
protected:
    void initialSetup() override;
    void onDraw() override;
    void onResize(int x, int y) override;

private:
    Camera camera;
    TextureManager& textureMan;

    std::vector<Sprite> sprites;

    GuiLayer guilayer;

    void initTextures();
};

int main(int argc, char** argv)
{
    TestEngine(argc, argv).run();
    return 0;
}

TestEngine::TestEngine(int argc, char** argv, const char* wname)
    : Engine(argc, argv, wname), camera(),
      textureMan(getResourceManager().getTextureManager()), sprites(),
      guilayer()
{
    onResize(getVieportWidth(), getVieportHeight());
}

void TestEngine::initialSetup()
{
    initTextures();

    auto& spriteTex = textureMan.get("data/images/aaa.png");
    sprites.resize(50);
    for (auto& s : sprites) {
        s.setTexture(spriteTex);
        s.transform.setScale(spriteTex.getWidth() / 2,
                             spriteTex.getHeight() / 2, 0);
        s.transform.move(randomVector(350));
    }

    auto vl     = new VerticalLayout();
    auto* panel = new Panel(vl, new ColorRectWidget(Color(0.1, 0.8, 0.6, 0.5)));
    // new ImageWidget(spriteTex.get()));
    panel->setPosition(20, 20);

    auto* w = new ImageWidget(spriteTex);
    w->setPosition(50, 50);

    auto* w1 = new ColorRectWidget(Color(0.8, 0.3, 0.1, 0.5));
    w1->setPosition(25, 30);
    w1->setSize(110, 60);

    auto* w2 = new ColorRectWidget(Color::FUCHSIA);
    w2->setPosition(35, 0);

    auto* w3 = new TextWidget("Прювет Лунатикам!!!");
    // auto* w3 = new TextWidget("Hello to lunatics!!!");
    w3->setHAlign(HALIGN::CENTER);
    w3->setVAlign(VALIGN::BOTTOM);
    w3->setColor(Color::WHITE);

    panel->addWidget(w3);
    panel->addWidget(w);
    panel->addWidget(w1);
    panel->addWidget(w2);
    panel->setSize(250, 220);

    auto button1 = new ButtonText("Button Left");
    auto button2 = new ButtonText("Button Center");
    auto button3 = new ButtonText("Button Right");

    auto hl = new HorizontalLayout();
    hl->setPadding(20);
    hl->addWidget(button1);
    hl->addWidget(button2);
    hl->addWidget(button3);
    panel->addWidget(hl);
    auto le = new LineEdit();
    le->addOnPressEnterAction([w3](const std::string& str) {
        const static auto quitlist = {"Exit", "exit", "quit", "q"};
        if (contain(quitlist, str))
            exit(EXIT_SUCCESS);
        w3->setText(str);
    });
    le->setSize(200, 30);

    button1->addOnClickAction([w3]() { w3->setText("You press Button 1"); });
    button2->addOnClickAction([w3]() { w3->setText("You press Button 2"); });
    button3->addOnClickAction([w3]() { w3->setText("You press Button 3"); });
    button1->addOnClickAction([le]() {
        le->setHAlign(HALIGN::LEFT);
        le->setFocus();
    });
    button2->addOnClickAction([le]() {
        le->setHAlign(HALIGN::CENTER);
        le->setFocus();
    });
    button3->addOnClickAction([le]() {
        le->setHAlign(HALIGN::RIGHT);
        le->setFocus();
    });

    panel->addWidget(le);
    guilayer.addWidget(panel);
}

void TestEngine::onDraw()
{
    drawSprites(*this, camera, sprites);
    guilayer.draw();

    const auto& kbd = getInput().getKeyboard();
    if (contain(kbd.getEvents(),
                InputEvent{INPUT_EVENT_TYPE::KEY_DOWN, SCANCODE_ESC}))
        guilayer.setVisible(!guilayer.getVisible());

    guilayer.processInputEvents(getInput());
}

void TestEngine::onResize(int x, int y)
{
    const int left   = -x / 2;
    const int right  = x + left;
    const int top    = y / 2;
    const int bottom = top - y;
    camera.setOrtho(left, right, bottom, top, -100, 100);
    guilayer.resize(x, y);
}

void TestEngine::initTextures()
{
    TextureAtlas ta(512, 512,
                    {
                        "data/images/image1.png", "data/images/image2.png",
                        "data/images/image3.png", "data/images/image4.png",
                        "data/images/image5.png", "data/images/image6.png",
                        "data/images/image7.png", "data/images/image8.png",
                        "data/images/image9.png", "data/images/image10.png",
                        "data/images/image11.png", "data/images/image12.png",
                        "data/images/image13.png", "data/images/image14.png",
                        "data/images/image15.png",
                    });
    textureMan.add("data/images/aaa.png");
    textureMan.addAtlas(std::move(ta));
}
