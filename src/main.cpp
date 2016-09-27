#include <memory>
#include <iomanip>
#include <string>
#include <core/engine.h>
#include <graphics/camera.h>
#include <graphics/textures/textureatlas.h>
#include <graphics/fonts/textline.h>
#include <std/draw.h>
#include <core/resourcemanager.h>


std::shared_ptr<Geometry> makeSpriteGeometry()
{
    static const GLfloat spriteVertices[] = { -0.5, -0.5,   0, 0,
                                              0.5,  0.5,   1, 1,
                                              -0.5,  0.5,   0, 1,
                                              0.5, -0.5,   1, 0 };
    static const size_t sizeOfVertices = sizeof(spriteVertices);
    static const GLuint spriteIndices[] = { 0,1,2, 0, 1, 3 };
    static const size_t numIndices =  sizeof(spriteIndices)
                                      /sizeof(spriteIndices[0]);
    return std::make_shared<Geometry>(
                spriteVertices, sizeOfVertices,
                spriteIndices,  numIndices,
                VertexAttributes({ VertexAttribDesc(ATTRIB_LOCATION::POSITION, 2, GL_FLOAT),
                                   VertexAttribDesc(ATTRIB_LOCATION::TEXCOORD, 2, GL_FLOAT) }),
                GL_TRIANGLES);
}


class TestEngine : public Engine
{
public:
    TestEngine(int argc, char** argv, const char* wname="TextEngine")
        : Engine(argc, argv, wname), resman(getResourceManager())
    {}
    // Engine interface
protected:

    void initialSetup(){
        camera.setOrtho(-100, 100, -100, 100, -100, 100);
        initGeometry();
        initShaders();
        initTextures();
        initFonts();
    }

    void onDraw(){
        auto& shaderMan = resman.getShaderManager();
        auto& textureMan = resman.getTextureManager();
        auto& geometryMan = resman.getGeometryManager();

        Transform textTransform;
        textTransform.move(0, -80, 0);
        drawText(camera, *shaderMan.get("text"), *textGeometry, textTransform,
                 *font, Colour::ORANGE);


        auto spriteTex = textureMan.get("data/images/aaa.png");
        const auto& spriteGeom = geometryMan.get("sprite");
        const auto& spriteShader = shaderMan.get("sprite");
        Transform spriteTransform;
        spriteTransform.setScale(spriteTex->getWidth(),
                                 spriteTex->getHeight(),
                                 0);
        drawTextured(camera, *spriteShader, *spriteGeom, spriteTransform,
                     *spriteTex);
    }

    void onResize(int x, int y)
    {
        const int left = -x/2;
        const int right = x + left;
        const int top  = y/2;
        const int bottom = top - y;
        camera.setOrtho(left, right, bottom, top, -100, 100);
    }

private:
    Camera camera;
    ResourceManager& resman;
    std::shared_ptr<Font>   font;
    std::shared_ptr<Geometry> textGeometry;

    void initShaders()
    {
        auto& shaderMan = resman.getShaderManager();
        shaderMan.add("sprite", "data/shaders/sprite.vsh",
                                "data/shaders/sprite.fsh");
        shaderMan.add("text", "data/shaders/text.vsh",
                              "data/shaders/text.fsh");
    }

    void initTextures()
    {
        auto& textureMan = resman.getTextureManager();
        TextureAtlas ta(512, 512, {
                            "data/images/aaa.png",
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
                        }, 0);
        textureMan.addAtlas(ta);
    }

    void initGeometry()
    {
        auto& geomMgr = resman.getGeometryManager();
        geomMgr.add("sprite", makeSpriteGeometry());
    }

    void initFonts()
    {
        auto& fontMan = resman.getFontManager();
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


