#include "shader.hpp"
#include "utils/helper.hpp"
#include <core/graphics/lowlevel/lowlevelshader.hpp>
#include <core/graphics/lowlevel/vertexattribute.hpp>
#include <memory>

using namespace vvv3d;

std::unique_ptr<Shader> Shader::fromStrings(const std::string& name,
                                            const char* vertexSource,
                                            const char* fragmentSource,
                                            const char* geometryShader)
{
    bench timings(std::string("making shader ") + name);
    // workaround std::make_shared with private constructor
    auto ret = std::make_unique<Shader>(_private{});
    ret->program.CreateProgram(
        LowLevelShader::fromString(vertexSource, GL_VERTEX_SHADER).get(),
        LowLevelShader::fromString(fragmentSource, GL_FRAGMENT_SHADER).get(),
        geometryShader
            ? LowLevelShader::fromString(geometryShader, GL_GEOMETRY_SHADER)
                  .get()
            : 0,
        bindAttribLocations);
    ret->fragmentSourceName = fragmentSource;
    ret->vertexSourceName = vertexSource;
    ret->loadLocations(name);
    return ret;
}

std::unique_ptr<Shader> Shader::fromFiles(const std::string& name,
                                          const char* vertexFileName,
                                          const char* fragmentFileName,
                                          const char* geometryFileName)
{
    bench timings("loading shader " + name);
    const auto gsh =
        geometryFileName
            ? LowLevelShader(geometryFileName, GL_GEOMETRY_SHADER).get()
            : 0;
    // workaround std::make_shared with private constructor
    auto ret = std::make_unique<Shader>(_private{});
    ret->program.CreateProgram(
        LowLevelShader(vertexFileName, GL_VERTEX_SHADER).get(),
        LowLevelShader(fragmentFileName, GL_FRAGMENT_SHADER).get(), gsh,
        bindAttribLocations);
    ret->fragmentSourceName = fragmentFileName;
    ret->vertexSourceName = vertexFileName;
    ret->loadLocations(name);
    return ret;
}

void Shader::activate() { program.activate(); }

#define LOC_(l) locations[static_cast<size_t>((l))]
#define CHECK_LOC()                                                            \
    if (loc == -1)                                                             \
        return;

void Shader::setModel(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::MODEL);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setView(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::VIEW);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setProjection(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::PROJECTION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setViewProjection(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::VIEW_PROJECITON);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setModelViewProjeciton(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::MODEL_VIEW_PROJECTION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setModelRotation(const vvv::matrix33f& m)
{
    const auto loc = LOC_(LOCATIONS::MODEL_ROTATION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setModelRotation(const vvv::matrix44f& m)
{
    const auto loc = LOC_(LOCATIONS::MODEL_ROTATION);
    CHECK_LOC()
    program.setUniform(loc, m);
}

void Shader::setColor(int n, const Color& color)
{
    const auto loc = LOC_(static_cast<size_t>(LOCATIONS::COLOR0) + n);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor0(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR0);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor1(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR1);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor2(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR2);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor3(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR3);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor4(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR4);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor5(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR5);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor6(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR6);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setColor7(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::COLOR7);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setTexturePosition(const vvv::vector4f& pos)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE_POSITION);
    CHECK_LOC();
    program.setUniform(loc, pos);
}

void Shader::setTexture(int n, const Texture& tex)
{
    const auto loc = LOC_(static_cast<size_t>(LOCATIONS::TEXTURE0) + n);
    CHECK_LOC()
    program.setUniform(loc, n);
    tex.bind(n);
}

void Shader::setTexture0(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE0);
    CHECK_LOC()
    program.setUniform(loc, 0);
    tex.bind(0);
}

void Shader::setTexture1(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE1);
    CHECK_LOC()
    program.setUniform(loc, 1);
    tex.bind(1);
}

void Shader::setTexture2(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE2);
    CHECK_LOC()
    program.setUniform(loc, 2);
    tex.bind(2);
}

void Shader::setTexture3(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE3);
    CHECK_LOC()
    program.setUniform(loc, 3);
    tex.bind(3);
}

void Shader::setTexture4(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE4);
    CHECK_LOC()
    program.setUniform(loc, 4);
    tex.bind(4);
}

void Shader::setTexture5(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE5);
    CHECK_LOC()
    program.setUniform(loc, 5);
    tex.bind(5);
}

void Shader::setTexture6(const Texture& tex)
{
    const auto loc = LOC_(LOCATIONS::TEXTURE6);
    CHECK_LOC()
    program.setUniform(loc, 6);
    tex.bind(6);
}

void Shader::setTexture7(const Texture& tex)
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

void Shader::setAmbientLightColor(const Color& color)
{
    const auto loc = LOC_(LOCATIONS::AMBIENT_COLOR);
    CHECK_LOC()
    program.setUniform(loc, color);
}

void Shader::setPosition(const vvv::vector4f& pos)
{
    const auto loc = LOC_(LOCATIONS::POSITION);
    CHECK_LOC();
    program.setUniform(loc, pos);
}

void Shader::setLights(const std::vector<vvv3d::Light>& lights)
{
    const auto lights_count_loc = LOC_(LOCATIONS::LIGHTS_COUNT);
    if (lights_count_loc == -1)
        return;

    constexpr size_t MAX_LIGHTS = 8;
    std::array<vvv::vector3f, MAX_LIGHTS> positions;
    std::array<vvv::vector3f, MAX_LIGHTS> directions;
    std::array<vvv3d::Color, MAX_LIGHTS> colors;
    std::array<GLint, MAX_LIGHTS> types;
    std::array<float, MAX_LIGHTS> intensities;
    std::array<float, MAX_LIGHTS> cutoffs;
    std::array<float, MAX_LIGHTS> exponents;

    const auto lights_size = lights.size();
    const size_t num_lights = std::min(MAX_LIGHTS, lights_size);
    if (num_lights < lights_size)
        std::cerr << "warning: too many lights passed to set (" << lights_size
                  << ") "
                  << " truncated to " << MAX_LIGHTS << "\n";

    for (size_t i = 0; i < num_lights; ++i) {
        positions[i] = lights[i].getPosition();
        directions[i] = lights[i].getDirection();
        colors[i] = lights[i].getColor();
        types[i] = static_cast<GLint>(lights[i].getType());
        intensities[i] = lights[i].getIntensity();
        cutoffs[i] = lights[i].getCutoff();
        exponents[i] = lights[i].getExponent();
    }

    const auto positions_loc = LOC_(LOCATIONS::LIGHT_POSITIONS);
    const auto directions_loc = LOC_(LOCATIONS::LIGHT_DIRECTIONS);
    const auto colors_loc = LOC_(LOCATIONS::LIGHT_COLORS);
    const auto types_loc = LOC_(LOCATIONS::LIGHT_TYPES);
    const auto intensities_loc = LOC_(LOCATIONS::LIGHT_INTENSITIES);
    const auto cutoffs_loc = LOC_(LOCATIONS::LIGHT_CUTOFFS);
    const auto exponents_loc = LOC_(LOCATIONS::LIGHT_EXPONENTS);

    if (positions_loc != -1)
        program.setUniform(positions_loc, positions.data(), num_lights);
    if (directions_loc != -1)
        program.setUniform(directions_loc, directions.data(), num_lights);
    if (colors_loc != -1)
        program.setUniform(colors_loc, colors.data(), num_lights);
    if (types_loc != -1)
        program.setUniform(types_loc, types.data(), num_lights);
    if (intensities_loc != -1)
        program.setUniform(intensities_loc, intensities.data(), num_lights);
    if (cutoffs_loc != -1)
        program.setUniform(cutoffs_loc, cutoffs.data(), num_lights);
    if (exponents_loc != -1)
        program.setUniform(exponents_loc, exponents.data(), num_lights);

    program.setUniform(lights_count_loc, static_cast<GLint>(num_lights));
}

const LowLevelShaderProgram& Shader::getLowLevelShader() const
{
    return program;
}

Shader::Shader() : program(), vertexSourceName(), fragmentSourceName() {}

GLint Shader::loadLocation(const char* name)
{
    GLint ret = program.getUniformLocation(name);
    if (ret != -1)
        std::cout << "\tuniform \"" << name << "\" found ..." << std::endl;
    return ret;
}

void Shader::loadLocations(const std::string& name)
{
    const size_t count = static_cast<size_t>(LOCATIONS::COUNT);
    activate();
    std::cout << "Searching uniforms in shader \"" << name << "\"\n";
    for (size_t i = 0; i < count; ++i)
        locations[i] = loadLocation(locations_names[i]);
}

const char* Shader::locations_names[] = {"modelMatrix",
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
                                         "position",
                                         "lightPositions",
                                         "lightDirections",
                                         "lightColors",
                                         "lightTypes",
                                         "lightIntensities",
                                         "lightCutoffs",
                                         "lightExponents",
                                         "lightsCount"};

ShaderManager::ShaderManager() : shaders() {}

void ShaderManager::add(const std::string& name,
                        const std::string& vertexShaderFilename,
                        const std::string& fragmentShaderFilename)
{
    if (contain(name))
        return;

    shaders[name] =
        Shader::fromFiles(name.c_str(), vertexShaderFilename.c_str(),
                          fragmentShaderFilename.c_str());
}

void ShaderManager::add(const std::string& name,
                        const std::string& vertexShaderFilename,
                        const std::string& fragmentShaderFilename,
                        const std::string& geometryShaderFilename)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromFiles(
        name.c_str(), vertexShaderFilename.c_str(),
        fragmentShaderFilename.c_str(), geometryShaderFilename.c_str());
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromStrings(
        name.c_str(), vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource,
                                  const std::string& geometryShaderSource)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromStrings(
        name.c_str(), vertexShaderSource.c_str(), fragmentShaderSource.c_str(),
        geometryShaderSource.c_str());
}

void ShaderManager::add(const std::string& name, std::unique_ptr<Shader> shader)
{
    if (contain(name))
        return;

    shaders[name].swap(shader);
}

void ShaderManager::add(const std::string& name,
                        const std::function<std::unique_ptr<Shader>()>& f)
{
    initializers[name] = f;
}

Shader& ShaderManager::get(const std::string& name) const try {
    return *shaders.at(name);
}
catch (std::exception& e) {
    std::cerr << "failed to get shader \'" << name << "\'\n";
    throw;
}

Shader& ShaderManager::get(const std::string& name) try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end())
            add(name, it->second());
    }
    return *shaders.at(name);
}
catch (std::exception& e) {
    std::cerr << "failed to get shader \'" << name << "\'\n";
    throw;
}

bool ShaderManager::contain(const std::string& name) const
{
    return shaders.find(name) != shaders.end();
}

std::vector<std::string> ShaderManager::listNames() const
{
    std::vector<std::string> ret;
    for (const auto& kv : shaders)
        ret.push_back(kv.first);
    return ret;
}
