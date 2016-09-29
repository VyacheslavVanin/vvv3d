#ifndef SHADER_H
#define SHADER_H
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <vvv3d/vvvmath/linalg.h>
#include <vvv3d/core/graphics/colour.h>
#include <vvv3d/core/graphics/textures/texture.h>
#include <vvv3d/core/graphics/lowlevel/lowlevelshaderprogram.h>

class Shader
{
private:

    Shader() = default;
    GLint loadLocation(const char* name);
    void loadLocations(const std::string &name);

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

    // part of workaround std::make_shared with private constructor
    struct _private{};

public:
    static std::shared_ptr<Shader> fromStrings(const std::string &name,
                                               const char* vertexSource,
                                               const char* fragmentSource);

    static std::shared_ptr<Shader> fromFiles(const std::string &name,
                                             const char* vertexFileName,
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
    void setTexture(int n, const Texture &tex);
    void setTexture0(const Texture& tex);
    void setTexture1(const Texture& tex);
    void setTexture2(const Texture& tex);
    void setTexture3(const Texture& tex);
    void setTexture4(const Texture& tex);
    void setTexture5(const Texture& tex);
    void setTexture6(const Texture& tex);
    void setTexture7(const Texture& tex);
    void setTime(float t);
    void setAmbientLightColor(const Colour& colour);
    void setLightPos(int n, const vvv::vector3f& pos);
    void setLightDir(int n, const vvv::vector3f& pos);
    void setLightColour(int n, const Colour& colour);

    // part of workaround std::make_shared with private constructor
    explicit Shader(const _private&) : Shader() {}
};

class ShaderManager
{
public:
    ShaderManager();

    void add(const std::string& name,
                   const std::string& vertexShaderFilename,
                   const std::string& fragmentShaderFilename);
    void add(const std::string& name, std::shared_ptr<Shader> shader);

    std::shared_ptr<Shader> get(const std::string& name) const;

    std::vector<std::string> listNames() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};


#endif // SHADER_H
