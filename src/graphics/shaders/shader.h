#ifndef SHADER_H
#define SHADER_H
#include <memory>
#include <vvvmath/linalg.h>
#include <graphics/colour.h>
#include <graphics/textures/texture.h>
#include <graphics/lowlevel/lowlevelshaderprogram.h>

class Shader
{
public:
    Shader();
    static std::shared_ptr<Shader> fromStrings(const char* vertexSource,
                                               const char* fragmentSource);

    static std::shared_ptr<Shader> fromFiles(const char* vertexFileName,
                                             const char* fragmentFileName);

    void activate();

    void setModel(const vvv::matrix44f& m);
    void setView(const vvv::matrix44f& m);
    void setProjection(const vvv::matrix44f& m);
    void setViewProjection(const vvv::matrix44f& m);
    void setModelViewProjeciton(const vvv::matrix44f& m);
    void setModelRotation(const vvv::matrix33f& m);
    void setColour(int n, const Colour& colour);
    void setColour0(const Colour& colour);
    void setColour1(const Colour& colour);
    void setColour2(const Colour& colour);
    void setColour3(const Colour& colour);
    void setColour4(const Colour& colour);
    void setColour5(const Colour& colour);
    void setColour6(const Colour& colour);
    void setColour7(const Colour& colour);
    void setTexturePosition(const vvv::vector4f& pos);
    void setTexture(int n, Texture &tex);
    void setTexture0(Texture& tex);
    void setTexture1(Texture& tex);
    void setTexture2(Texture& tex);
    void setTexture3(Texture& tex);
    void setTexture4(Texture& tex);
    void setTexture5(Texture& tex);
    void setTexture6(Texture& tex);
    void setTexture7(Texture& tex);
    void setTime(float t);
    void setAmbientColor(const Colour& colour);
    void setLightPos(int n, const vvv::vector3f& pos);
    void setLightDir(int n, const vvv::vector3f& pos);
    void setLightColour(int n, const Colour& colour);

private:
    GLint loadLocation(const char* name);
    void loadLocations();

    LowLevelShaderProgram program;
    std::string vertexSourceName;
    std::string fragmentSourceName;

    enum class LOCATIONS{
        MODEL,
        VIEW,
        PROJECTION,
        VIEW_PROJECITON,
        MODEL_VIEW_PROJECTION,
        MODEL_ROTATION,
        COLOUR0,
        COLOUR1,
        COLOUR2,
        COLOUR3,
        COLOUR4,
        COLOUR5,
        COLOUR6,
        COLOUR7,
        TEXTURE_POSITION,
        TEXTURE0,
        TEXTURE1,
        TEXTURE2,
        TEXTURE3,
        TEXTURE4,
        TEXTURE5,
        TEXTURE6,
        TEXTURE7,
        TIME,
        AMBIENT_COLOUR,
        COUNT };
    static const char* locations_names[static_cast<size_t>(LOCATIONS::COUNT)];

    GLint locations[static_cast<size_t>(LOCATIONS::COUNT)];
};

#endif // SHADER_H
