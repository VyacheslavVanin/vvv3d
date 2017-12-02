#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <vvv3d/core/graphics/color.h>
#include <vvv3d/core/graphics/lowlevel/lowlevelshaderprogram.h>
#include <vvv3d/core/graphics/textures/texture.h>
#include <vvv3d/vvvmath/linalg.h>

namespace vvv3d {

class Shader {
private:
    Shader();
    GLint loadLocation(const char* name);
    void loadLocations(const std::string& name);

    LowLevelShaderProgram program;
    std::string vertexSourceName;
    std::string fragmentSourceName;

    enum class LOCATIONS {
        MODEL,
        VIEW,
        PROJECTION,
        VIEW_PROJECITON,
        MODEL_VIEW_PROJECTION,
        MODEL_ROTATION,
        COLOR0,
        COLOR1,
        COLOR2,
        COLOR3,
        COLOR4,
        COLOR5,
        COLOR6,
        COLOR7,
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
        AMBIENT_COLOR,
        POSITION,
        COUNT
    };
    static const char* locations_names[static_cast<size_t>(LOCATIONS::COUNT)];
    GLint locations[static_cast<size_t>(LOCATIONS::COUNT)];

    // part of workaround std::make_shared with private constructor
    struct _private {
    };

    static std::unique_ptr<Shader> fromStrings(const std::string& name,
                                               const char* vertexSource,
                                               const char* fragmentSource,
                                               const char* geometrySource = 0);

    static std::unique_ptr<Shader> fromFiles(const std::string& name,
                                             const char* vertexFileName,
                                             const char* fragmentFileName,
                                             const char* geometryFileName = 0);
    friend class ShaderManager;

public:
    void activate();

    void setModel(const vvv::matrix44f& m);
    void setView(const vvv::matrix44f& m);
    void setProjection(const vvv::matrix44f& m);
    void setViewProjection(const vvv::matrix44f& m);
    void setModelViewProjeciton(const vvv::matrix44f& m);
    void setModelRotation(const vvv::matrix33f& m);
    void setColor(int n, const Color& color);
    void setColor0(const Color& color);
    void setColor1(const Color& color);
    void setColor2(const Color& color);
    void setColor3(const Color& color);
    void setColor4(const Color& color);
    void setColor5(const Color& color);
    void setColor6(const Color& color);
    void setColor7(const Color& color);
    void setTexturePosition(const vvv::vector4f& pos);
    void setTexture(int n, const Texture& tex);
    void setTexture0(const Texture& tex);
    void setTexture1(const Texture& tex);
    void setTexture2(const Texture& tex);
    void setTexture3(const Texture& tex);
    void setTexture4(const Texture& tex);
    void setTexture5(const Texture& tex);
    void setTexture6(const Texture& tex);
    void setTexture7(const Texture& tex);
    void setTime(float t);
    void setAmbientLightColor(const Color& color);
    void setPosition(const vvv::vector4f& pos);
    void setLightPos(int n, const vvv::vector3f& pos);
    void setLightDir(int n, const vvv::vector3f& pos);
    void setLightColor(int n, const Color& color);

    const LowLevelShaderProgram& getLowLevelShader() const;

    // part of workaround std::make_shared with private constructor
    explicit Shader(const _private&) : Shader() {}
};

class ShaderManager {
public:
    ShaderManager();

    void add(const std::string& name, const std::string& vertexShaderFilename,
             const std::string& fragmentShaderFilename);
    void add(const std::string& name, const std::string& vertexShaderFilename,
             const std::string& fragmentShaderFilename,
             const std::string& geometryShaderFilename);
    void add(const std::string& name, std::unique_ptr<Shader> shader);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource,
                       const std::string& geometryShaderSource);

    Shader& get(const std::string& name) const;

    std::vector<std::string> listNames() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};
}

