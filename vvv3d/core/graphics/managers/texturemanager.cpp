#include "texturemanager.hpp"
#include "utils/helper.hpp"
#include "vvvstlhelper.hpp"

#include <sstream>

#include <vvv3d/std/texture_io.hpp>

using namespace vvv3d;

namespace {
const std::string DEFAULT_TEXTURE_NAME = "default";
}

TextureManager::TextureManager() : BaseManagerSafe<Texture>()
{
    addLL(vvv3d::makeDummyTexture(256, 256, 32), DEFAULT_TEXTURE_NAME);
    addCreateFunction([](const std::string& filename) {
        auto* texture = readTexture(filename);
        if (texture == nullptr)
            throw std::runtime_error("failed to load from file");

        return std::make_shared<Texture>(texture);
    });
}

void TextureManager::addLL(LowLevelTexture* texture, const std::string& name)
{
    if (contain(name))
        return;
    addLLForce(texture, name);
}

void TextureManager::addLLForce(LowLevelTexture* texture,
                                const std::string& name)
{
    addForce(name, std::make_unique<Texture>(texture));
}

void TextureManager::addFromFile(const std::string& filename,
                                 const std::string& name)
{
    if (contain(name))
        return;

    auto* llt = readTexture(filename);
    if (llt == nullptr)
        throw std::runtime_error("failed to load " + filename + " as " + name);
    std::shared_ptr<LowLevelTexture> im(llt);
    auto texture = std::make_shared<Texture>(im);
    addForce(name, texture);
    addForce(filename, std::move(texture));
}

void TextureManager::addFromFile(const std::string& filename)
{
    addFromFile(filename, filename);
}

void TextureManager::safe_add(const std::string& filename)
try {
    addFromFile(filename);
}
catch (const std::exception& e) {
    std::cerr << "TextureManager: Failed to load \"" << filename << "\"\n";
    std::cerr << "\t\"" << e.what() << "\"\n";
}
catch (...) {
    std::cerr << "TextureManager: Failed to load \"" << filename << "\"\n";
}

void TextureManager::addAtlas(std::unique_ptr<TextureAtlas> atlas)
{
    addAtlas(std::move(*atlas));
}

void TextureManager::addAtlas(
    std::vector<std::unique_ptr<TextureAtlas>>&& atlases)
{
    for (auto& a : atlases)
        addAtlas(std::move(a));
}

void TextureManager::addAtlas(TextureAtlas&& atlas)
{
    for (auto i = atlas.textures.begin(); i != atlas.textures.end();) {
        add(i->first, std::move(i->second));
        i = atlas.textures.erase(i);
    }
}

namespace {
void loadAtlassesFromDict(vvv3d::TextureManager& tm, const vvv::Value& images,
                          long size, long border)
{
    std::vector<std::string> names;
    std::vector<std::string> filenames;
    const auto& dict = images.asDict();

    for (const auto& name_uri : dict) {
        const auto& name = name_uri.first;
        const auto& uri = name_uri.second;
        if (!uri.isString()) {
            std::cerr << "Skip load \"" << name
                      << "\" cause uri is not string\n";
            continue;
        }

        names.push_back(name);
        filenames.push_back(uri.asString());
    }
    auto atlases =
        vvv3d::TextureAtlas::pack(size, size, filenames, names, border);
    tm.addAtlas(std::move(atlases));
}

void loadAtlassesFromList(vvv3d::TextureManager& tm, const vvv::Value& images,
                          long size, long border)
{
    std::vector<std::string> names;
    const auto& dict = images.asList();

    for (const auto& name : dict) {
        if (!name.isString()) {
            std::cerr << "Skip load \"" << name
                      << "\" cause uri is not string\n";
            continue;
        }

        names.push_back(name.asString());
    }
    auto atlases = vvv3d::TextureAtlas::pack(size, size, names, border);
    tm.addAtlas(std::move(atlases));
}

void loadAtlasFromPackedTexture(vvv3d::TextureManager& tm,
                                const vvv::CfgNode& atlas_node)
{
    const auto& atlas_name = atlas_node.getName();
    const vvv::Value& texture_property = atlas_node.getProperty("texture");
    if (!texture_property.isString()) {
        std::cerr << "Error: skip loading atlas " << atlas_name
                  << ": \"texture\" property has non string type\n";
        return;
    }

    const vvv::Value& images = atlas_node.getProperty("images");
    if (!images.isDict()) {
        std::cerr << "Error: skip loading atlas " << atlas_name
                  << ": \"images\" property has non dict type\n";
        return;
    }

    const auto& filename = texture_property.asString();
    std::vector<std::string> names;
    std::vector<vvv::vector4f> coords;
    const auto& dict = images.asDict();
    names.reserve(dict.size());
    coords.reserve(dict.size());
    for (const auto& item : dict) {
        const auto& name = item.first;
        const auto& value = item.second;
        if (!value.isList()) {
            std::cerr << "Error: skip loading atlas " << atlas_name
                      << ": one of \"images\" value has non list type\n";
            return;
        }
        if (value.size() != 4) {
            std::cerr << "Error: skip loading atlas " << atlas_name
                      << ": one of \"images\" value has size != 4\n";
            return;
        }
        const auto& string_list = value.asStringList();
        const vvv::vector4f coord(
            to_float(string_list[0]), to_float(string_list[1]),
            to_float(string_list[2]), to_float(string_list[3]));
        names.push_back(name);
        coords.push_back(coord);
    }

    tm.addAtlas(vvv3d::TextureAtlas::makeAtlas(filename, coords, names));
}

void loadAtlasesSection(vvv3d::TextureManager& tm, const vvv::CfgNode& cfg)
{
    vvv3d::bench timing("loading atlases section");
    if (!cfg.hasChild("atlases"))
        return;

    const auto& atlases = cfg.getChild("atlases");
    for (const auto& atlas : atlases.getChildren()) {
        if (!atlas.hasProperty("images")) {
            std::cerr << "Skip \"" << atlas.getName()
                      << "\" record of atlasses cause no \"images\" property\n";
            continue;
        }
        const auto& images = atlas.getProperty("images");
        const auto& type = images.getType();

        if (atlas.hasProperty("texture")) {
            loadAtlasFromPackedTexture(tm, atlas);
            continue;
        }

        const auto& size = atlas.getPropertyAsLong("size", 512);
        const auto& border = atlas.getPropertyAsLong("border", 0);

        switch (type) {
        case vvv::Value::DATA_TYPE::DICT:
            loadAtlassesFromDict(tm, images, size, border);
            break;
        case vvv::Value::DATA_TYPE::LIST:
            loadAtlassesFromList(tm, images, size, border);
            break;
        default:
            std::cerr << "Skip \"" << atlas.getName()
                      << "\" record of atlasses cause \"images\" property has "
                         "invalid type (list or dict expected)\n";
            continue;
        }
    }
}

void loadStringEntry(vvv3d::TextureManager& tm, const std::string& name,
                     const vvv::Value& value)
{
    tm.addFromFile(value.asString(), name);
}

void loadDictEntry(vvv3d::TextureManager& tm, const vvv::Value& value)
{
    for (const auto& name_val : value.asDict()) {
        const auto& name = name_val.first;
        const auto& val = name_val.second;
        if (!val.isString()) {
            std::cerr << "Invalid type of dict value (string expected)\n";
            continue;
        }
        const auto& file_name = val.asString();
        tm.addFromFile(file_name, name);
    }
}

void loadListEntry(vvv3d::TextureManager& tm, const vvv::Value& value)
{
    for (const auto& val : value.asList()) {
        if (!val.isString()) {
            std::cerr << "Invalid type of list value (string expected)\n";
            continue;
        }
        const auto& file_name = val.asString();
        tm.addFromFile(file_name);
    }
}

void loadImagesSection(vvv3d::TextureManager& tm, const vvv::CfgNode& cfg)
{
    vvv3d::bench timing("loading images section");
    if (!cfg.hasChild("images"))
        return;

    const auto& images = cfg.getChild("images");
    for (const auto& entry : images.getChildren()) {
        const auto& name = entry.getName();
        vvv3d::bench timing("loading " + name);
        const auto& value = entry.getValue();
        const auto& value_type = value.getType();
        switch (value_type) {
        case vvv::Value::DATA_TYPE::STRING:
            loadStringEntry(tm, name, value);
            break;
        case vvv::Value::DATA_TYPE::DICT: loadDictEntry(tm, value); break;
        case vvv::Value::DATA_TYPE::LIST: loadListEntry(tm, value); break;
        }
    }
}

} // namespace

void TextureManager::load(const vvv::CfgNode& cfg)
{
    vvv3d::bench timing("loading textures from config");
    if (!cfg.hasChild("textures"))
        return;

    const auto& textures = cfg.getChild("textures");
    loadImagesSection(*this, textures);
    loadAtlasesSection(*this, textures);
}

void TextureManager::load(const std::string& string)
{
    std::stringstream ss;
    ss << string;
    load(ss);
}

void TextureManager::load(std::istream& stream)
{
    const auto& cfg = vvv::make_cfg(stream);
    load(cfg);
}
