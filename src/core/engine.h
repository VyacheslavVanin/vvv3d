#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <graphics/fonts/font.h>
#include <graphics/textures/texturemanager.h>
#include <graphics/geometry.h>
#include <graphics/shaders/shader.h>

class ResourceManager;

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

    int getVieportWidth() const;
    int getVieportHeight() const;

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static std::shared_ptr<Engine>  activeEngine;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    int viewportWidth;
    int viewportHeight;
    std::unique_ptr<ResourceManager> resourceManager;
};
#endif // ENGINE

