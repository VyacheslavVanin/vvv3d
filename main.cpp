#include <memory>
#include <iomanip>
#include <string>
#include <vvv3d/vvv3d.h>
#include <gui/guilayer.h>
#include <gui/imagewidget.h>
#include <gui/colorrectanglewidget.h>
#include <gui/textwidget.h>
#include <gui/verticallayout.h>
#include <gui/horizontallayout.h>
#include <gui/panel.h>
#include <gui/buttonbase.h>
#include <gui/buttontext.h>
#include <random>

vvv::vector3f randomVector(float range)
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-range, range);
    return {uniform_dist(e1),uniform_dist(e1), 0};
}

std::shared_ptr<Geometry> makeSpriteGeometry()
{
    static const GLfloat spriteVertices[] = { -0.5, -0.5,   0, 0,
                                               0.5,  0.5,   1, 1,
                                              -0.5,  0.5,   0, 1,
                                               0.5, -0.5,   1, 0 };
    static const size_t sizeOfVertices    = sizeof(spriteVertices);
    static const GLuint spriteIndices[]   = { 0,1,2, 0, 1, 3 };
    static const size_t numIndices        =  sizeof(spriteIndices)
                                            /sizeof(spriteIndices[0]);
    return std::make_shared<Geometry>(
                spriteVertices, sizeOfVertices,
                spriteIndices,  numIndices,
                VertexAttributes({ VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
                                   VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT) }),
                GL_TRIANGLES);
}

class Sprite
{
public:
    Sprite() : transform(), texture(nullptr) {}
    Sprite(const Sprite&) = default;
    Sprite& operator=(const Sprite&) = default;
    ~Sprite() = default;

    void setTexture(Texture* texture) {this->texture = texture;}
    Texture* getTexture() const {return texture;}

    Transform transform;
private:
    Texture* texture;
};

void drawSprite(Engine& engine, const Camera& camera, const Sprite& spr){
    auto& resman = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman = resman.getGeometryManager();
    auto sh = shaderman.get("sprite");
    auto g = geomman.get("sprite");

    drawTextured(camera, *sh, *g, spr.transform, *spr.getTexture());
}

template<typename A, template<typename, typename> class C >
void drawSprites(Engine& engine, const Camera& camera, const C<Sprite, A>& sprs)
{
    auto& resman = engine.getResourceManager();
    auto& shaderman = resman.getShaderManager();
    auto& geomman = resman.getGeometryManager();
    auto sh = shaderman.get("sprite");
    auto g = geomman.get("sprite");

    sh->activate();
    sh->setViewProjection(camera.getViewProjection());
    for(const auto& s : sprs){
        const auto texture = s.getTexture();
        sh->setModel(s.transform.getModelMatrix());
        sh->setTexturePosition(texture->getTexturePosition());
        sh->setTexture0(*texture);
        g->draw();
    }
}

class TestEngine : public Engine
{
public:
    TestEngine(int argc, char** argv, const char* wname="TextEngine");
    // Engine interface
protected:
    void initialSetup() override;
    void onDraw() override;
    void onResize(int x, int y) override;

private:
    Camera camera;
    ShaderManager&      shaderMan;
    TextureManager&     textureMan;
    GeometryManager&    geometryMan;
    FontManager&        fontMan;

    std::vector<Sprite> sprites;

    std::shared_ptr<Font>       font;
    std::shared_ptr<Geometry>   textGeometry;

    GuiLayer guilayer;

    void initShaders();
    void initTextures();
    void initGeometry();
    void initFonts();
};

int main(int argc, char** argv)
{
    TestEngine(argc, argv).run();
    return 0;
}

class TestWidget : public TextWidget
{
public:
    TestWidget() : TextWidget("Idle")
    {
        setSize(150, 50);
        setFocusable(true);
    }

    // Widget interface
protected:
    void OnGetFocus() override
    {
        setText("Get focus");
    }

    void OnLoseFocus() override
    {
        setText("lost focus");
    }
};


TestEngine::TestEngine(int argc, char** argv, const char* wname)
    : Engine(argc, argv, wname),
      camera(),
      shaderMan(getResourceManager().getShaderManager()),
      textureMan(getResourceManager().getTextureManager()),
      geometryMan(getResourceManager().getGeometryManager()),
      fontMan(getResourceManager().getFontManager()),
      sprites(), font(), textGeometry(), guilayer()
{}

void TestEngine::initialSetup() {
    initGeometry();
    initShaders();
    initTextures();
    initFonts();

    auto spriteTex = textureMan.get("data/images/aaa.png");
    sprites.resize(50);
    for(auto& s : sprites){
        s.setTexture(spriteTex.get());
        s.transform.setScale(spriteTex->getWidth()/2,
                             spriteTex->getHeight()/2,
                             0);
        s.transform.move(randomVector(350));
    }

    auto* panel = new Panel(
                      new VerticalLayout(),
                      new ColorRectWidget(Color(0.1, 0.8, 0.6, 0.5)));
    // new ImageWidget(spriteTex.get()));
    panel->setPosition(20, 20);

    auto* w = new ImageWidget(spriteTex.get());
    w->setPosition(50, 50);

    auto* w1 = new ColorRectWidget(Color(0.8, 0.3, 0.1, 0.5));
    w1->setPosition(25, 30);
    w1->setSize(110, 60);

    auto* w2 = new ColorRectWidget(Color::FUCHSIA);
    w2->setPosition(35, 0);

    auto* w3 = new TextWidget("Прювет Лунатикам!!!");
    //auto* w3 = new TextWidget("Hello to lunatics!!!");
    w3->setHAlign(HALIGN::CENTER);
    w3->setVAlign(VALIGN::BOTTOM);
    w3->setColor(Color::WHITE);

    panel->addWidget(w3);
    panel->addWidget(w);
    panel->addWidget(w1);
    panel->addWidget(w2);
    panel->setSize(250, 220);

    auto button1 = new ButtonText("Button 1");
    button1->addOnClickAction([w3](){w3->setText("You press Button 1");});
    auto button2 = new ButtonText("Button 2");
    button2->addOnClickAction([w3](){w3->setText("You press Button 2");});

    auto hl = new HorizontalLayout();
    hl->setPadding(20);
    hl->addWidget(button1);
    hl->addWidget(button2);
    panel->addWidget(hl);
    panel->addWidget(new TestWidget());
    panel->addWidget(new TestWidget());
    guilayer.addWidget(panel);
}

void TestEngine::onDraw()
{
    drawSprites(*this, camera, sprites);
    guilayer.draw();
    guilayer.processInputEvents(getInput());

    //auto& i = getInput();
    //auto pos = i.getMouse().getMouseRel();
    //if(pos.x != 0 || pos.y != 0)
    //    std::cout << i.getMouse().getMouseRel() << "\n";
    //if(i.getKeyboard().hasText())
    //    std::cout << i.getKeyboard().getText() << "\n";
}

void TestEngine::onResize(int x, int y)
{
    const int left = -x/2;
    const int right = x + left;
    const int top  = y/2;
    const int bottom = top - y;
    camera.setOrtho(left, right, bottom, top, -100, 100);
    guilayer.resize(x,y);
}

void TestEngine::initShaders()
{
    shaderMan.add("sprite", "data/shaders/sprite.vsh",
                  "data/shaders/sprite.fsh");
}

void TestEngine::initTextures()
{
    TextureAtlas ta(512, 512, {
                        "data/images/image1.png",
                        "data/images/image2.png",
                        "data/images/image3.png",
                        "data/images/image4.png",
                        "data/images/image5.png",
                        "data/images/image6.png",
                        "data/images/image7.png",
                        "data/images/image8.png",
                        "data/images/image9.png",
                        "data/images/image10.png",
                        "data/images/image11.png",
                        "data/images/image12.png",
                        "data/images/image13.png",
                        "data/images/image14.png",
                        "data/images/image15.png",
                    });
    textureMan.add("data/images/aaa.png");
    textureMan.addAtlas(ta);
}

void TestEngine::initGeometry()
{
    geometryMan.add("sprite", makeSpriteGeometry());
}

void TestEngine::initFonts()
{
}
