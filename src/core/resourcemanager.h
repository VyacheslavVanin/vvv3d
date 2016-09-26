#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <graphics/fonts/font.h>
#include <graphics/textures/texturemanager.h>
#include <graphics/geometry.h>
#include <graphics/shaders/shader.h>

class ResourceManager
{
public:
    TextureManager&  getTextureManager()    {return textureManager;}
    FontManager&     getFontManager()       {return fontManager;}
    ShaderManager&   getShaderManager()     {return shaderManger;};
    GeometryManager& getGeometryManager()   {return geometryManager;};
private:
    TextureManager   textureManager;
    FontManager      fontManager;
    ShaderManager    shaderManger;
    GeometryManager  geometryManager;
};

#endif // RESOURCEMANAGER_H
