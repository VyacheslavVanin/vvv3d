#include <memory>
#include <iomanip>
#include <string>
#include <core/layeredengine.h>
#include <std/spritelayer.h>
#include <graphics/textures/textureatlas.h>
#include <graphics/fonts/textline.h>
#include <std/draw.h>

class SpriteEngine : public LayeredEngine
{
public:
    SpriteEngine(int argc, char** argv, const char* windowName="SpriteEngine")
        : LayeredEngine(argc,argv, windowName),
          layer(new SpriteLayer(this)),
          s()
    {
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
                           /* "data/images/image16.png",
                            "data/images/image17.png",
                            "data/images/image18.png",
                            "data/images/image19.png",
                            "data/images/image20.png",
                            "data/images/image21.png",
                            "data/images/image22.png",
                            "data/images/image23.png",
                            "data/images/image24.png",
                            "data/images/image25.png",
                            "data/images/image26.png",
                            "data/images/image27.png",
                            "data/images/image28.png",
                            "data/images/image29.png",
                            "data/images/image30.png",
                            "data/images/image31.png",
                            "data/images/image32.png",
                            "data/images/image33.png",
                            "data/images/image34.png",
                            "data/images/image35.png",
                            "data/images/image36.png",
                            "data/images/image37.png",
                            "data/images/image38.png",
                            "data/images/image39.png",
                            "data/images/image40.png",
                            "data/images/image41.png",
                            "data/images/image42.png",
                            "data/images/image43.png",
                            "data/images/image44.png",
                            "data/images/image45.png",
                            "data/images/image46.png",
                            "data/images/image47.png",
                            "data/images/image48.png",
                            "data/images/image49.png",
                            "data/images/image50.png",
                            "data/images/image51.png",
                            "data/images/image52.png",
                            "data/images/image53.png",
                            "data/images/image54.png",
                            "data/images/image55.png",
                            "data/images/image56.png",
                            "data/images/image57.png",
                            "data/images/image58.png",
                            "data/images/image59.png",
                            "data/images/image60.png",
                            "data/images/image61.png",
                            "data/images/image62.png",
                            "data/images/image63.png",
                            "data/images/image64.png",
                            "data/images/image65.png",
                            "data/images/image66.png",
                            "data/images/image67.png",
                            "data/images/image68.png",
                            "data/images/image69.png",
                            "data/images/image70.png",
                            "data/images/image71.png",
                            "data/images/image72.png",
                            "data/images/image73.png",
                            "data/images/image74.png",
                            "data/images/image75.png",
                            "data/images/image76.png",
                            "data/images/image77.png",
                            "data/images/image78.png",
                            "data/images/image79.png",
                            "data/images/image80.png",
                            "data/images/image81.png",
                            "data/images/image82.png",
                            "data/images/image83.png",
                            "data/images/image84.png",
                            "data/images/image85.png",
                            "data/images/image86.png",
                            "data/images/image87.png",
                            "data/images/image88.png",
                            "data/images/image89.png",
                            "data/images/image90.png",
                            "data/images/image91.png",
                            "data/images/image92.png",
                            "data/images/image93.png",
                            "data/images/image94.png",
                            "data/images/image95.png",
                            "data/images/image96.png",
                            "data/images/image97.png",
                            "data/images/image98.png",
                            "data/images/image99.png",
                            */
                        }, 0);

        getTextureManager().addAtlas(ta);

        addLayer( layer );
        s = std::make_shared<Sprite>();
        s->setTexture( getTextureManager().get("data/images/aaa.png"));
        s->transform.setScale(s->getTexture().getWidth(), s->getTexture().getHeight(), 1);
        //auto tt = ta.get("data/images/image10.png");
        //s->setTexture(tt);
        //s->transform.setScale(tt->getWidth(),tt->getHeight(), 1);
        //s->transform.scale(4);
        layer->add(s);


        auto& fm = getFontManager();
        fm.addFont("default", "data/fonts/DejaVuSans.ttf", 20);
        font = fm.getFont("default");
        textGeometry = createTextGeometry(*font, std::u32string(U"Hello World"));

        getShaderManager().add("text", "data/shaders/text.vsh", "data/shaders/text.fsh");

    }

protected:
    void onDraw()
    {
        LayeredEngine::onDraw();

        Transform textTransform;
        textTransform.move(0, -80, 0);

        drawText(layer->getCamera(), *getShaderManager().get("text"),
                 *textGeometry, textTransform, *font, Colour::ORANGE);

    //    s->transform.rotate(0.01, 0, 0, 1);
    }

private:
    std::shared_ptr<SpriteLayer> layer;
    std::shared_ptr<Sprite> s;
    std::shared_ptr<Font>   font;
    std::shared_ptr<Geometry> textGeometry;
    // Engine interface
};


int main(int argc, char** argv)
{
    SpriteEngine* engine = new SpriteEngine(argc,argv);
    engine->run();

    return 0;
}


