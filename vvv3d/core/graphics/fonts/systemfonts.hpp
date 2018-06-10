#pragma once
#include <string>
#include <vector>

namespace vvv3d {

class FontDesc {
public:
    FontDesc(const std::string& filename, const std::string& family,
             const std::string& style, const std::string& format);

    const std::string& getFileName() const;
    const std::string& getFamily() const;
    const std::string& getStyle() const;
    const std::string& getFormat() const;

private:
    std::string filename;
    std::string family;
    std::string style;
    std::string format;
};

class SystemFonts {
public:
    static const SystemFonts& instance();
    SystemFonts(const SystemFonts&) = delete;
    SystemFonts(SystemFonts&&) = delete;
    SystemFonts& operator=(const SystemFonts&) = delete;
    SystemFonts& operator=(SystemFonts&&) = delete;

    const std::vector<FontDesc>& getFontDescList() const;
    const FontDesc* getDefaultRegular() const;
    const FontDesc* getDefaultBold() const;
    const FontDesc* getDefaultItalic() const;
    const FontDesc* getDefaultMono() const;
    const FontDesc* getDesc(const char* family, const char* style) const;

private:
    SystemFonts();
    const FontDesc* loadFirstOflist(
        const std::vector<std::pair<const char*, const char*>>& list,
	const FontDesc* fallback=nullptr) const;
    std::vector<FontDesc> allFonts;
    const FontDesc* defaultRegular;
    const FontDesc* defaultBold;
    const FontDesc* defaultItalic;
    const FontDesc* defaultMono;
};

const SystemFonts& systemFonts();
} // namespace vvv3d
