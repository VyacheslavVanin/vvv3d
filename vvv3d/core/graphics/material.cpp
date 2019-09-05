#include "material.hpp"

#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/managers/shader_manager.hpp>
#include <vvvstdhelper/containerhelper.hpp>

namespace vvv3d {

namespace {
static const std::string kDiffuseStr = "diffuse";
static const std::string kEmissionStr = "emission";
static const std::string kNormalStr = "normal";
static const std::string kSpecularStr = "specular";
static const std::string kRoughnessStr = "roughness";
static const std::string kMetallicStr = "metallic";

/**
 * @brief Substitute first occurance of \p pattern in \p data with \n new_str
 * @param data
 * @param pattern
 * @param new_str
 * @return count of substitutions */
size_t substitute(std::string& data, const std::string& pattern,
                  const std::string& new_str)
{
    const auto pos = data.find(pattern);
    if (pos == std::string::npos)
        return 0;

    data.replace(pos, pattern.size(), new_str);

    return 1;
}

std::string decorateTemplateVar(const std::string& str)
{
    static const std::string prefix = "${";
    static const std::string postfix = "}";
    static const size_t prefix_postfix_size = prefix.size() + postfix.size();
    const size_t s = str.size() + prefix_postfix_size;
    std::string ret;
    ret.reserve(s);
    ret += prefix;
    ret += str;
    ret += postfix;
    return ret;
}

static const Material::ValueSources defaultSources = {
    // SoftWhite (3c)
    {Material::PROPERTY::DIFFUSE, Material::SOURCE_TYPE::NONE},
    {Material::PROPERTY::EMISSION, Material::SOURCE_TYPE::NONE}, // Black (3c)
    // va_normal
    {Material::PROPERTY::NORMAL, Material::SOURCE_TYPE::NONE},
    // Black, power 0.0
    {Material::PROPERTY::SPECULAR, Material::SOURCE_TYPE::NONE},
    {Material::PROPERTY::ROUGHNESS, Material::SOURCE_TYPE::NONE}, // White (1c)
    {Material::PROPERTY::METALLIC, Material::SOURCE_TYPE::NONE},  // Black (1c)
};

Material::ValueSources
updatedValueSources(const Material::ValueSources& user_overrides)
{
    Material::ValueSources ret = defaultSources;
    for (const auto& source : user_overrides) {
        const auto& name = source.first;
        const auto& user_value = source.second;
        auto it = ret.find(name);
        if (it != ret.end())
            it->second = user_value;
    }

    return ret;
}

struct ValueSourcesMappings {
    std::vector<uint8_t> textures_indices;
    std::vector<uint8_t> color_indices;
    size_t textures_count = 0;
    size_t colors_count = 0;
};

ValueSourcesMappings
makeValueSourcesMapping(const Material::ValueSources& sources)
{
    static const size_t properties_count =
        static_cast<size_t>(Material::PROPERTY::COUNT);

    ValueSourcesMappings ret;
    ret.textures_indices = std::vector<uint8_t>(properties_count, 0xffu);
    ret.color_indices = std::vector<uint8_t>(properties_count, 0xffu);

    const auto& all_sources = updatedValueSources(sources);
    for (size_t i = 0; i < properties_count; ++i) {
        const auto& property = static_cast<Material::PROPERTY>(i);
        const auto& source = all_sources.at(property);
        switch (source) {
        case Material::SOURCE_TYPE::TEXTURE:
            ret.textures_indices[i] = ret.textures_count++;
            break;
        case Material::SOURCE_TYPE::COLOR:
            ret.color_indices[i] = ret.colors_count++;
            break;
        case Material::SOURCE_TYPE::NONE: break;
        }
    }

    return ret;
}

// static const std::string prefix = "    uniform sampler2D texture";
std::string makeTemplateValue(size_t count, const std::string& prefix)
{
    static const std::string postfix = ";\n";

    static const constexpr size_t max_digits = 3;
    const size_t size = count * (prefix.size() + postfix.size() + max_digits);

    std::string ret;
    ret.reserve(size);

    for (size_t i = 0; i < count; ++i) {
        ret += prefix;
        ret += std::to_string(i);
        ret += postfix;
    }

    return ret;
}

// TODO: replace value_type type with PROPERTY_TYPE of material
// Value type
// 0 - diffuse
// 1 - emission
// 2 - normal
// 3 - specular
// 4 - roughnes
// 5 - metallic
std::string makeTemplateValue(uint8_t value_type,
                              const ValueSourcesMappings& mappings,
                              const std::string& fallback)
{
    // TODO: add special handling for "normal map"
    // (need to do some precalculations)
    const auto& color_index = mappings.color_indices.at(value_type);
    if (color_index != 0xffu)
        return "color" + std::to_string(color_index);

    const auto& texture_index = mappings.textures_indices.at(value_type);
    if (texture_index != 0xffu)
        return "texture(texture" + std::to_string(texture_index) +
               ", vsout_texCoord)";

    return fallback;
}

const std::string& to_string(Material::PROPERTY p)
{
    switch (p) {
    case Material::PROPERTY::DIFFUSE: return kDiffuseStr;
    case Material::PROPERTY::EMISSION: return kEmissionStr;
    case Material::PROPERTY::NORMAL: return kNormalStr;
    case Material::PROPERTY::SPECULAR: return kSpecularStr;
    case Material::PROPERTY::ROUGHNESS: return kRoughnessStr;
    case Material::PROPERTY::METALLIC: return kMetallicStr;
    default: throw std::logic_error("Invalid Material::Property");
    }
}

const std::string& to_string(Material::SOURCE_TYPE type)
{
    static const std::string kTexture = "Texture";
    static const std::string kColor = "Color";
    static const std::string kNone = "None";
    switch (type) {
    case Material::SOURCE_TYPE::TEXTURE: return kTexture;
    case Material::SOURCE_TYPE::COLOR: return kColor;
    case Material::SOURCE_TYPE::NONE: return kNone;
    }
    throw std::logic_error("Missing SOURCE_TYPE");
}

std::string makeName(const Material::ValueSources& sources)
{
    static const size_t properties_count =
        static_cast<size_t>(Material::PROPERTY::COUNT);

    const auto& all_sources = updatedValueSources(sources);
    std::string ret;
    ret += "generated_material_";
    for (size_t i = 0; i < properties_count; ++i) {
        const auto prop = static_cast<Material::PROPERTY>(i);
        const auto& type = to_string(prop);
        const auto& name = to_string(all_sources.at(prop));
        ret += name;
        ret += '_';
        ret += type;
        ret += ':';
    }
    return ret;
}

std::string makeVertShader(const ValueSourcesMappings& mappings)
{
    static const char* vsh_template = R"(
    #version 300 es
    uniform mat4 viewProjectionMatrix;
    uniform mat4 modelMatrix;
    uniform mat4 modelRotation;

    in vec3 va_position;
    in vec3 va_normal;
    in vec2 va_texCoord;
    ${va_tangent_decl}
    ${va_bitangent_decl}

    out highp vec4 vsout_normal;
    out highp vec4 vsout_position;
    out highp vec2 vsout_texCoord;
    ${tbn_output_decl}

    void main(void)
    {
        vsout_position = modelMatrix * vec4(va_position, 1.0f);
        vsout_texCoord = va_texCoord;
        ${tbn_calculation}

        vsout_normal = modelRotation * vec4(va_normal, 1.0f);
        gl_Position = viewProjectionMatrix * vsout_position;
    }
    )";
    std::string ret(vsh_template);
    const bool use_normal_map = mappings.textures_indices[2] != 0xff;
    if (!use_normal_map) {
        static const std::string empty = "";
        substitute(ret, decorateTemplateVar("va_tangent_decl"), empty);
        substitute(ret, decorateTemplateVar("va_bitangent_decl"), empty);
        substitute(ret, decorateTemplateVar("tbn_output_decl"), empty);
        substitute(ret, decorateTemplateVar("tbn_calculation"), empty);
        return ret;
    }

    static const std::string va_tangent_decl = R"(in vec3 va_tangent;)";
    static const std::string va_bitangent_decl = R"(in vec3 va_bitangent;)";
    static const std::string tbn_output_decl = R"(out highp mat4 vsout_tbn;)";
    static const std::string tbn_calculation = R"(
    vec4 T = normalize(modelRotation * vec4(va_tangent, 0.0f));
    vec4 B = normalize(modelRotation * vec4(va_bitangent, 0.0f));
    vec4 N = normalize(modelRotation * vec4(va_normal, 0.0f));
    vsout_tbn = mat4(T, B, N, vec4(0, 0, 0, 1));
    )";
    substitute(ret, decorateTemplateVar("va_tangent_decl"), va_tangent_decl);
    substitute(ret, decorateTemplateVar("va_bitangent_decl"),
               va_bitangent_decl);
    substitute(ret, decorateTemplateVar("tbn_output_decl"), tbn_output_decl);
    substitute(ret, decorateTemplateVar("tbn_calculation"), tbn_calculation);

    return ret;
}

std::string makeFragShader(const ValueSourcesMappings& mappings)
{
    static const char* fsh_template = R"(
    #version 300 es
    precision mediump float;
    uniform float time;
    
    // textures
    // uniform sampler2D texture0;
    ${textures}

    // colors
    // uniform vec4 color0;
    ${colors}

    in vec4 vsout_normal;
    in vec4 vsout_position;
    in vec2 vsout_texCoord;
    ${tbn_input_decl}

    layout(location=0)out vec4 out_diffuse;
    layout(location=1)out vec4 out_position;
    layout(location=2)out vec4 out_normal;
    layout(location=3)out vec4 out_emission;
    layout(location=4)out vec4 out_specular;

    void main(void)
    {
        out_diffuse = ${diffuse};   // Diffuse color
        out_emission = ${emission};
        out_specular = ${specular};

        out_position = vsout_position;  // Use 4th component

        out_normal = ${normal};      // Use 4th component
        ${calculte_normal}

        // roughness
        out_normal.w = ${roughness}.x;
        // metalic
        out_specular.w = ${metallic}.x;
    }
    )";

    std::string fsh_str(fsh_template);
    substitute(fsh_str, decorateTemplateVar("textures"),
               makeTemplateValue(mappings.textures_count,
                                 "    uniform sampler2D texture"));
    substitute(
        fsh_str, decorateTemplateVar("colors"),
        makeTemplateValue(mappings.colors_count, "    uniform vec4 color"));
    substitute(fsh_str, decorateTemplateVar(kDiffuseStr),
               makeTemplateValue(0, mappings, "vec4(0.9f, 0.9f, 0.9f, 1.0f)"));
    substitute(fsh_str, decorateTemplateVar(kEmissionStr),
               makeTemplateValue(1, mappings, "vec4(0.0f, 0.0f, 0.0f, 0.0f)"));
    substitute(fsh_str, decorateTemplateVar(kNormalStr),
               makeTemplateValue(2, mappings, "vsout_normal"));
    substitute(fsh_str, decorateTemplateVar(kSpecularStr),
               makeTemplateValue(3, mappings, "vec4(0.0f, 0.0f, 0.0f, 0.0f)"));
    substitute(fsh_str, decorateTemplateVar(kRoughnessStr),
               makeTemplateValue(4, mappings, "vec4(0.96f, 0.0f, 0.0f, 0.0f)"));
    substitute(fsh_str, decorateTemplateVar(kMetallicStr),
               makeTemplateValue(5, mappings, "vec4(0.0f, 0.0f, 0.0f, 0.0f)"));

    const bool use_normal_map = mappings.textures_indices[2] != 0xff;
    if (!use_normal_map) {
        static const std::string empty = "";
        substitute(fsh_str, decorateTemplateVar("tbn_input_decl"), empty);
        substitute(fsh_str, decorateTemplateVar("calculte_normal"), empty);
    }
    else {
        static const std::string tbn_input_decl = R"(in mat4 vsout_tbn;)";
        static const std::string calculte_normal = R"(
            out_normal = vec4(normalize(out_normal.xyz * 2.0 - 1.0), 1.0);   
            out_normal.y *= -1.0;
            out_normal = vec4(normalize((vsout_tbn * out_normal).xyz), 1.0);
        )";
        substitute(fsh_str, decorateTemplateVar("tbn_input_decl"),
                   tbn_input_decl);
        substitute(fsh_str, decorateTemplateVar("calculte_normal"),
                   calculte_normal);
    }

    std::cout << fsh_str << "\n";

    return fsh_str;
}

} // namespace

Material::Material(const Material::ValueSources& outputs, ShaderManager& shm)
{
    auto mappings = makeValueSourcesMapping(outputs);
    const auto& name = makeName(outputs);
    if (!shm.contain(name)) {
        const auto& vsh = makeVertShader(mappings);
        const auto& fsh = makeFragShader(mappings);
        auto shader =
            vvv3d::Shader::fromStrings(name, vsh.c_str(), fsh.c_str());
        shm.add(name, std::move(shader));
    }

    this->shader = shm.getShared(name);
    this->textures_indices = std::move(mappings.textures_indices);
    this->colors_indices = std::move(mappings.color_indices);
    this->textures = std::vector<TextureShared>(mappings.textures_count);
    this->colors = std::vector<Color>(mappings.colors_count);
}

Material& Material::setDiffuse(TextureShared texture)
{
    return setTexture(PROPERTY::DIFFUSE, std::move(texture));
}

Material& Material::setEmission(TextureShared texture)
{
    return setTexture(PROPERTY::EMISSION, std::move(texture));
}

Material& Material::setNormal(TextureShared texture)
{
    return setTexture(PROPERTY::NORMAL, std::move(texture));
}

Material& Material::setSpecular(TextureShared texture)
{
    return setTexture(PROPERTY::SPECULAR, std::move(texture));
}

Material& Material::setRoughness(TextureShared texture)
{
    return setTexture(PROPERTY::ROUGHNESS, std::move(texture));
}

Material& Material::setMetallic(TextureShared texture)
{
    return setTexture(PROPERTY::METALLIC, std::move(texture));
}

Material& Material::setDiffuse(const Color& color)
{
    return setColor(PROPERTY::DIFFUSE, color);
}

Material& Material::setEmission(const Color& color)
{
    return setColor(PROPERTY::EMISSION, color);
}

Material& Material::setNormal(const Color& color)
{
    return setColor(PROPERTY::NORMAL, color);
}

Material& Material::setSpecular(const Color& color)
{
    return setColor(PROPERTY::SPECULAR, color);
}

Material& Material::setRoughness(const Color& color)
{
    return setColor(PROPERTY::ROUGHNESS, color);
}

Material& Material::setMetallic(const Color& color)
{
    return setColor(PROPERTY::METALLIC, color);
}

Shader& Material::getShader() { return *shader; }

const std::vector<TextureShared>& Material::getTextures() const
{
    return textures;
}

const std::vector<Color>& Material::getColors() const { return colors; }

Material& Material::setTexture(Material::PROPERTY index,
                               TextureShared&& texture)
{
    const auto i = getIndex(index);
    if (i == std::numeric_limits<uint8_t>::max())
        return *this;

    const auto& texture_index = textures_indices[i];
    textures[texture_index] = std::move(texture);
    return *this;
}

Material& Material::setColor(Material::PROPERTY index, const Color& color)
{
    const auto i = getIndex(index);
    if (i == std::numeric_limits<uint8_t>::max())
        return *this;

    const auto& color_index = colors_indices[i];
    colors[color_index] = color;
    return *this;
}

uint8_t Material::getIndex(Material::PROPERTY index) const
{
    return static_cast<uint8_t>(index);
}

void Material::activate()
{
    if (shader == nullptr)
        return;

    shader->activate();
    for (size_t i = 0; i < colors.size(); ++i) {
        shader->setColor(i, colors[i]);
    }
    for (size_t i = 0; i < textures.size(); ++i) {
        const auto& texture = textures[i];
        if (texture != nullptr)
            shader->setTexture(i, *texture);
    }
}

Material::Material(const Material::ValueSources& outputs)
    : Material(outputs, vvv3d::getShaderManager())
{
}
} // namespace vvv3d
