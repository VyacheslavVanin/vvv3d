#include "font_desc.hpp"
#include <vector>

namespace vvv3d {

FontDesc::FontDesc(const std::string& filename, const std::string& family,
                   const std::string& style, const std::string& format)
    : filename(filename), family(family), style(style), format(format)
{
}

const std::string& FontDesc::getFileName() const { return filename; }
const std::string& FontDesc::getFamily() const { return family; }
const std::string& FontDesc::getStyle() const { return style; }
const std::string& FontDesc::getFormat() const { return format; }

} // namespace vvv3d
