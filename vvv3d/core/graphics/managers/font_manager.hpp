#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/fonts/font.hpp>
#include <vvv3d/core/graphics/fonts/font_desc.hpp>

namespace vvv3d {
class FontManager {
public:
    FontManager();
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = default;
    FontManager& operator=(FontManager&&) = default;
    ~FontManager();

    void addFont(const std::string& name, const std::string& filename,
                 unsigned int fontsize);
    void addFont(const std::string& name, const FontDesc& desc,
                 unsigned int fontsize);
    const Font& getFont(const std::string& name) const;
    std::vector<std::string> listNames() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
};
} // namespace vvv3d
