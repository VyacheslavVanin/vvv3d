#include "shader.h"
#include <memory>
#include <core/graphics/lowlevel/vertexattribute.h>
#include <core/graphics/lowlevel/lowlevelshader.h>

std::shared_ptr<Shader> Shader::fromStrings(const std::string& name,
                                            const char* vertexSource,
                                            const char* fragmentSource)
{
    // workaround std::make_shared with private constructor
    auto ret = std::make_shared<Shader>(_private{});
    ret->program.CreateProgram(
         LowLevelShader::fromString(vertexSource, GL_VERTEX_SHADER).get(),
         LowLevelShader::fromString(fragmentSource, GL_FRAGMENT_SHADER).get(),
         bindAttribLocations);
    ret->fragmentSourceName = fragmentSource;
    ret->vertexSourceName = vertexSource;
    ret->loadLocations(name);
    return ret;
}

std::shared_ptr<Shader> Shader::fromFiles(const std::string& name,
                                          const char* vertexFileName,
                                          const char* fragmentFileName)
{
    // workaround std::make_shared with private constructor
    auto ret = std::make_shared<Shader>(_private{});
    ret->program.CreateProgram( LowLevelShader(vertexFileName,
                                               GL_VERTEX_SHADER).get(),
                                LowLevelShader(fragmentFileName,
                                               GL_FRAGMENT_SHADER).get(),
                                bindAttribLocations);
    ret->fragmentSourceName = fragmentFileName;
    ret->vertexSourceName = vertexFileName;
    ret->loadLocations(name);
    return ret;
}

void Shader::activate()
{
    program.activate();
}


#define LOC_(l) locations[static_cast<size_t>((l))]
#define CHECK_LOC() if(loc==-1)return;

void Shader::setModel(const vvv::matrix44f &m)
{
    const auto loc = LOC_(LOCATIONS::MODEL);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setView(const vvv::matrix44f &m)
{
    const auto loc = LOC_(LOCATIONS::VIEW);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setProjection(const vvv::matrix44f &m)
{
    const auto loc = LOC_(LOCATIONS::PROJECTION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setViewProjection(const vvv::matrix44f &m)
{
    const auto loc = LOC_(LOCATIONS::VIEW_PROJECITON);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setModelViewProjeciton(const vvv::matrix44f &m)
{
    const auto loc = LOC_(LOCATIONS::MODEL_VIEW_PROJECTION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setModelRotation(const vvv::matrix33f &m)
{
    const auto loc = LOC_(LOCATIONS::MODEL_ROTATION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setColor(int n, const Color &color)
{
    const auto loc = LOC_(static_cast<size_t>(LOCATIONS::COLOR0)+n);
    CHECK_LOC()
    program.setUniform(loc,
                       color.vec);
}

void Shader::setColor0(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR0);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor1(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR1);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor2(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR2);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor3(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR3);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor4(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR4);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor5(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR5);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor6(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR6);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setColor7(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::COLOR7);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setTexturePosition(const vvv::vector4f &pos)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE_POSITION);
    CHECK_LOC();
    program.setUniform(loc, pos);
}

void Shader::setTexture(int n, const Texture &tex)
{
    const auto loc = LOC_(static_cast<size_t>(LOCATIONS::TEXTURE0)+n);
    CHECK_LOC()
    program.setUniform(loc,n);
    tex.bind(n);
}

void Shader::setTexture0(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE0);
    CHECK_LOC()
    program.setUniform(loc, 0);
    tex.bind(0);
}

void Shader::setTexture1(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE1);
    CHECK_LOC()
    program.setUniform(loc, 1);
    tex.bind(1);
}

void Shader::setTexture2(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE2);
    CHECK_LOC()
    program.setUniform(loc, 2);
    tex.bind(2);
}

void Shader::setTexture3(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE3);
    CHECK_LOC()
    program.setUniform(loc, 3);
    tex.bind(3);
}

void Shader::setTexture4(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE4);
    CHECK_LOC()
    program.setUniform(loc, 4);
    tex.bind(4);
}

void Shader::setTexture5(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE5);
    CHECK_LOC()
    program.setUniform(loc, 5);
    tex.bind(5);
}

void Shader::setTexture6(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE6);
    CHECK_LOC()
    program.setUniform(loc, 6);
    tex.bind(6);
}

void Shader::setTexture7(const Texture &tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE7);
    CHECK_LOC()
    program.setUniform(loc, 7);
    tex.bind(7);
}

void Shader::setTime(float t)
{
    const auto loc = LOC_(LOCATIONS::TIME);
    CHECK_LOC()
    program.setUniform(loc, t);
}

void Shader::setAmbientLightColor(const Color &color)
{
    const auto loc = LOC_(LOCATIONS::AMBIENT_COLOR);
    CHECK_LOC()
    program.setUniform(loc, color.vec);
}

void Shader::setPosition(const vvv::vector4f& pos)
{
    const auto loc = LOC_(LOCATIONS::POSITION);
    CHECK_LOC();
    program.setUniform(loc, pos);
}

void Shader::setLightPos(int, const vvv::vector3f&)
{
    std::cout << "Shader::setLightPos not implemented yet" << std::endl;
}

void Shader::setLightDir(int, const vvv::vector3f&)
{
    std::cout << "Shader::setLightDir not implemented yet" <<std::endl;
}

void Shader::setLightColor(int, const Color &)
{
    std::cout << "Shader::setLightColor not implemented yet" <<std::endl;
}

GLint Shader::loadLocation(const char *name)
{
    GLint ret = program.getUniformLocation(name);
    if(ret != -1)
        std::cout << "\tuniform \"" << name << "\" ..." << std::endl;
    return ret;
}

void Shader::loadLocations(const std::string& name) {
    const size_t count = static_cast<size_t>(LOCATIONS::COUNT);
    activate();
    std::cout << "Searching uniforms in \"" << name << "\""
              << "and \"" << fragmentSourceName << "\": ..." <<std::endl;
    for(size_t i=0; i < count; ++i)
        locations[i] = loadLocation(locations_names[i]);
}

const char* Shader::locations_names[]=
{
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
    "viewProjectionMatrix",
    "modelViewProjectionMatrix",
    "modelRotation",
    "color0",
    "color1",
    "color2",
    "color3",
    "color4",
    "color5",
    "color6",
    "color7",
    "texturePosition",
    "texture0",
    "texture1",
    "texture2",
    "texture3",
    "texture4",
    "texture5",
    "texture6",
    "texture7",
    "time",
    "ambientColor",
    "position"
};

ShaderManager::ShaderManager() : shaders(){}

void ShaderManager::add(const std::string &name,
                        const std::string &vertexShaderFilename,
                        const std::string &fragmentShaderFilename)
{
    shaders[name] = Shader::fromFiles(name.c_str(),
                                      vertexShaderFilename.c_str(),
                                      fragmentShaderFilename.c_str());
}

void ShaderManager::add(const std::string &name, std::shared_ptr<Shader> shader)
{
    shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderManager::get(const std::string &name) const
{
    return shaders.at(name);
}

std::vector<std::string> ShaderManager::listNames() const
{
    std::vector<std::string> ret;
    for(auto kv: shaders)
        ret.push_back(kv.first);
    return ret;
}