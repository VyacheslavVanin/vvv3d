#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <vvv3d/core/graphics/fonts/font.h>
#include <vvv3d/core/graphics/textures/texturemanager.h>
#include <vvv3d/core/graphics/geometry.h>
#include <vvv3d/core/graphics/shaders/shader.h>

class ResourceManager
{
public:
    ResourceManager() :
        textureManager(), fontManager(), shaderManger(), geometryManager()
    {}

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
