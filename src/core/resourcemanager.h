#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <core/graphics/fonts/font.h>
#include <core/graphics/textures/texturemanager.h>
#include <core/graphics/geometry.h>
#include <core/graphics/shaders/shader.h>

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
