#include "shader.hpp"
#include "utils/helper.hpp"
#include <core/graphics/lowlevel/lowlevelshader.hpp>
#include <core/graphics/lowlevel/vertexattribute.hpp>
#include <memory>
#include <vvv3d/std/log.hpp>
#include <vvv3d/vvvmath/functions.hpp>

using namespace vvv3d;
using vvv::helper::format;

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
    vvv::vector3f positions[MAX_LIGHTS];
    vvv::vector3f directions[MAX_LIGHTS];
    vvv3d::Color colors[MAX_LIGHTS];
    GLint types[MAX_LIGHTS];
    float intensities[MAX_LIGHTS];
    float cutoffs[MAX_LIGHTS];
    float exponents[MAX_LIGHTS];

    const auto lights_size = lights.size();
    const size_t num_lights = std::min(MAX_LIGHTS, lights_size);
    if (num_lights < lights_size)
        LOG(format("warning: too many lights passed to set (@) truncated to @",
                   lights_size, MAX_LIGHTS));

    for (size_t i = 0; i < num_lights; ++i) {
        positions[i] = lights[i].getPosition();
        directions[i] = lights[i].getDirection();
        colors[i] = lights[i].getColor();
        types[i] = static_cast<GLint>(lights[i].getType());
        intensities[i] = lights[i].getIntensity();
        cutoffs[i] = cos(vvv::deg2radians(lights[i].getCutoff() / 2));
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
        program.setUniform(positions_loc, positions, num_lights);
    if (directions_loc != -1)
        program.setUniform(directions_loc, directions, num_lights);
    if (colors_loc != -1)
        program.setUniform(colors_loc, colors, num_lights);
    if (types_loc != -1)
        program.setUniform(types_loc, types, num_lights);
    if (intensities_loc != -1)
        program.setUniform(intensities_loc, intensities, num_lights);
    if (cutoffs_loc != -1)
        program.setUniform(cutoffs_loc, cutoffs, num_lights);
    if (exponents_loc != -1)
        program.setUniform(exponents_loc, exponents, num_lights);

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
        LOG(format("\tuniform \"@\" found ...", name));

    return ret;
}

void Shader::loadLocations(const std::string& name)
{
    const size_t count = static_cast<size_t>(LOCATIONS::COUNT);
    activate();
    LOG(format("Searching uniforms in shader \"@\"", name));
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

