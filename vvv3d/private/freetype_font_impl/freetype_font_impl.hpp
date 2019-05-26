#pragma once
#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>

namespace vvv3d {

std::unique_ptr<IFont> makeFreetypeFont(const std::string& font_file_name,
                                        unsigned int size,
                                        unsigned int charSize,
                                        unsigned int dpi);

} // namespace vvv3d
