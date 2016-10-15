#include <memory>
#include <iomanip>
#include <string>
#include <vvv3d/vvv3d.h>
#include <gui/guilayer.h>
#include <gui/imagewidget.h>
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
    Sprite() : transform() {}
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
    TestEngine(int argc, char** argv, const char* wname="TextEngine")
        : Engine(argc, argv, wname),
          shaderMan(getResourceManager().getShaderManager()),
          textureMan(getResourceManager().getTextureManager()),
          geometryMan(getResourceManager().getGeometryManager()),
          fontMan(getResourceManager().getFontManager())
    {}
    // Engine interface
protected:

    void initialSetup() override {
        camera.setOrtho(-100, 100, -100, 100, -100, 100);
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

        ImageWidget* w = new ImageWidget();
        w->setTexture(spriteTex.get());
        w->setSize(vvv::vector2f(100,100));
        w->setPosition(vvv::vector2f(50,50));

        guilayer.addWidget(w);
    }

    void onDraw() override {
        Transform textTransform;
        textTransform.move(0, -80, 0);
        drawText(camera, *shaderMan.get("text"), *textGeometry, textTransform,
                 *font, Colour::ORANGE);

        //drawSprites(*this, camera, sprites);
        guilayer.draw();
    }

    void onResize(int x, int y) override
    {
        const int left = -x/2;
        const int right = x + left;
        const int top  = y/2;
        const int bottom = top - y;
        camera.setOrtho(left, right, bottom, top, -100, 100);
        guilayer.resize(x,y);
    }

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

    void initShaders()
    {
        shaderMan.add("sprite", "data/shaders/sprite.vsh",
                                "data/shaders/sprite.fsh");
        shaderMan.add("text", "data/shaders/text.vsh",
                              "data/shaders/text.fsh");
        shaderMan.add("solid", "data/shaders/solidcolor.vsh",
                              "data/shaders/solidcolor.fsh");
    }

    void initTextures()
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

    void initGeometry()
    {
        geometryMan.add("sprite", makeSpriteGeometry());
    }

    void initFonts()
    {
        fontMan.addFont("default", "data/fonts/DejaVuSans.ttf", 20);
        font = fontMan.getFont("default");
        textGeometry = createTextGeometry(*font, std::u32string(U"Hello World"));
    }
};

int main(int argc, char** argv)
{
    Engine* engine = new TestEngine(argc,argv);
    engine->run();

    return 0;
}
