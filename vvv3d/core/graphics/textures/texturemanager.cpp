#include "texturemanager.hpp"
#include "utils/helper.hpp"

using namespace vvv3d;

TextureManager::TextureManager() : texs()
{
    add(vvv3d::makeDummyTexture(256, 256, 32), "default");
}

Texture& TextureManager::get(const std::string& name)
{
    auto i = texs.find(name);
    if (i != texs.end())
        return *i->second;

    safe_add(name);

    const auto* cthis = this;
    return cthis->get(name);
}

Texture& TextureManager::get(const std::string& name) const
{
    auto i = texs.find(name);
    if (i != texs.end())
        return *i->second;

    return getDefault();
}

void TextureManager::add(LowLevelTexture* texture, const std::string& name)
{
    if (contain(name))
        return;
    texs[name].reset(new Texture(texture));
}

void TextureManager::add(const std::string& filename, const std::string& name)
{
    if (contain(name))
        return;

    const auto& it = texs.find(filename);
    if (it != texs.end()) {
        texs[name] = it->second;
        return;
    }

    std::shared_ptr<LowLevelTexture> im(makeLLTexture(filename));
    auto texture = std::make_shared<Texture>(im);
    texs[name] = texture;
    texs[filename] = texture;
}

void TextureManager::add(const std::string& filename)
{
    add(filename, filename);
}

void TextureManager::safe_add(const std::string& filename) try {
    add(filename);
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
        texs.insert(std::move(*i));
        i = atlas.textures.erase(i);
    }
}

bool TextureManager::contain(const std::string& name) const
{
    return texs.find(name) != texs.end();
}

void TextureManager::remove(const std::string& name)
{
    auto i = texs.find(name);
    if (i != texs.end())
        texs.erase(i);
}

void TextureManager::clear() { texs.clear(); }

std::vector<std::string> TextureManager::listNames() const
{
    std::vector<std::string> ret;
    for (auto& kv : texs)
        ret.push_back(kv.first);
    return ret;
}

Texture& TextureManager::getDefault() const { return get("default"); }

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
    tm.add(value.asString(), name);
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
        tm.add(file_name, name);
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
        tm.add(file_name);
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
