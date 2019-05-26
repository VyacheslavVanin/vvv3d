#pragma once
#include <string>
#include <vector>

namespace vvv3d {

class FontDesc {
public:
    FontDesc() = default;
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

} // namespace vvv3d
