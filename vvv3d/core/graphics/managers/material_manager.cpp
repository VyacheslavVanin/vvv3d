#include "material_manager.hpp"
#include "utils/helper.hpp"
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/managers/texturemanager.hpp>
#include <vvv3d/core/graphics/material.hpp>

namespace vvv3d {
MaterialManager::MaterialManager(ShaderManager& shm)
    : BaseManagerSafe<Material>()
{
    auto material = std::make_shared<Material>(Material::ValueSources{}, shm);
    addForce(getDefaultKey(), std::move(material));
}

using material_node_parse_result =
    std::unordered_map<vvv3d::Material::PROPERTY,
                       std::pair<vvv3d::Material::SOURCE_TYPE, vvv::Any>>;

bool is_file_name(const std::string& s)
{
    const auto& i = s.rfind('.');
    if (i == std::string::npos)
        return false;

    // TODO: add to engine query suported image types
    static const std::vector<std::string> image_extensions = {
        ".png",
        ".jpeg",
        ".jpg",
    };
    const auto ext = s.substr(i);
    if (vvv::helpers::contain(image_extensions, ext))
        return true;

    return false;
}

material_node_parse_result getMaterialProperties(const vvv::CfgNode& node)
{
    using PROPERTY = vvv3d::Material::PROPERTY;
    using SOURCE_TYPE = vvv3d::Material::SOURCE_TYPE;

    material_node_parse_result ret;

    const size_t properies_count = static_cast<size_t>(PROPERTY::COUNT);
    for (auto i = 0; i < properies_count; ++i) {
        const auto property_type = static_cast<PROPERTY>(i);
        const auto& property_name = vvv3d::to_string(property_type);
        if (!node.hasProperty(property_name)) {
            ret[property_type] =
                std::make_pair(SOURCE_TYPE::NONE, vvv::Value());
            continue;
        }
        const auto& property = node.getProperty(property_name);
        const auto& property_value_type = property.getType();
        if (property_value_type == vvv::Value::DATA_TYPE::LIST) {
            const auto& color = to_color(property.asStringList());
            ret[property_type] =
                std::make_pair(SOURCE_TYPE::COLOR, vvv::Any(color));
            continue;
        }
        if (property_value_type == vvv::Value::DATA_TYPE::STRING) {
            const auto& property_string = property.asString();
            bool ok;
            const auto& color = to_color(property_string, ok);
            if (ok) {
                ret[property_type] =
                    std::make_pair(SOURCE_TYPE::COLOR, vvv::Any(color));
                continue;
            }

            ret[property_type] =
                std::make_pair(SOURCE_TYPE::TEXTURE, vvv::Any(property_string));
            continue;
        }
    }

    return ret;
}

Material::ValueSources toValueSources(const material_node_parse_result& in)
{
    Material::ValueSources ret;
    ret.reserve(in.size());
    for (const auto& v : in)
        ret[v.first] = v.second.first;

    return ret;
}

void MaterialManager::load(const vvv::CfgNode& cfg)
{
    using PROPERTY = vvv3d::Material::PROPERTY;
    using SOURCE_TYPE = vvv3d::Material::SOURCE_TYPE;

    static const std::string kMaterials = "materials";
    vvv3d::bench timing("loading textures from config");
    if (!cfg.hasChild(kMaterials))
        return;

    const auto& materials = cfg.getChild(kMaterials);
    for (const auto& entry : materials.getChildren()) {
        const auto& name = entry.getName();
        const auto& properties = entry.getProperties();
        const auto& material_desc = getMaterialProperties(entry);
        const auto& value_sources = toValueSources(material_desc);
        auto material = std::make_shared<Material>(value_sources);
        addForce(name, material);
        for (const auto& value_source : material_desc) {
            const auto& prop = value_source.first;
            const auto& source_type = value_source.second.first;
            const auto& value = value_source.second.second;
            if (source_type == SOURCE_TYPE::COLOR) {
                material->setColor(prop, vvv::any_cast<vvv3d::Color>(value));
                continue;
            }
            if (source_type == SOURCE_TYPE::TEXTURE) {
                auto& tm = vvv3d::getTextureManager();
                const auto& texture_name = vvv::any_cast<std::string>(value);
                auto texture = tm.getShared(texture_name);
                texture->setMipmapFilter();
                material->setTexture(prop, std::move(texture));
                continue;
            }
        }
    }
}

void MaterialManager::load(const std::string& string)
{
    std::stringstream ss;
    ss << string;
    load(ss);
}

void MaterialManager::load(std::istream& stream)
{
    const auto& cfg = vvv::make_cfg(stream);
    load(cfg);
}
} // namespace vvv3d
