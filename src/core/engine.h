#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <graphics/fonts/font.h>
#include <graphics/textures/texture.h>
#include <graphics/geometry.h>
#include <graphics/shaders/shader.h>

#define DRAW_TRANSPARENT for(auto trenable = [](){glDisable(GL_DEPTH_TEST); glEnable(GL_BLEND); \
                                                  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);return true;}(); \
                             trenable; \
                             glEnable(GL_DEPTH_TEST),glDisable(GL_BLEND),trenable=false)

class Engine
{
public:
    Engine(int argc, char** argv, const char* windowName = "vvv3d");

    virtual ~Engine();

    void run();

    float getCurrentFps() const;

    void keyboardFunc(unsigned char c, int s, int d);
    void keyboardUpFunc(unsigned char c, int s, int d);

    TextureManager&     getTextureManager();
    FontManager&        getFontManager();
    ShaderManager&      getShaderManager();
    GeometryManager&    getGeometryManager();
    static std::shared_ptr<Engine> getActiveEngine();

    int getVieportWidth() const;
    int getVieportHeight() const;

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static std::shared_ptr<Engine>  activeEngine;
    TextureManager                  textureManager;
    FontManager                     fontManager;
    ShaderManager                   shaderManger;
    GeometryManager                 geometryManager;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    int viewportWidth;
    int viewportHeight;
};
#endif // ENGINE

