#include "mgrfreetype.hpp"

namespace vvv3d {

MgrFreetype::MgrFreetype() : library(), fonts()
{
    if (FT_Init_FreeType(&library))
        throw std::logic_error("Error init freetype");
}

MgrFreetype::~MgrFreetype() { FT_Done_FreeType(library); }

const FT_Face& MgrFreetype::addFont(const std::string& name,
                                    const std::string& fontPath)
{
    auto name_it = fonts.find(name);
    if (name_it != fonts.end())
        return *name_it->second;

    auto path_it = fonts.find(fontPath);
    if (path_it != fonts.end()) {
        fonts[name] = path_it->second;
        return *path_it->second;
    }

    auto face = std::make_shared<FT_Face>();
    const FT_Error error =
        FT_New_Face(library, fontPath.c_str(), 0, face.get());
    if (error == FT_Err_Unknown_File_Format)
        throw std::logic_error(
            "the font file could be opened and read, but it appears"
            " that its font format is unsupported");
    else if (error)
        throw std::logic_error("the font file could not be opened or read, or "
                               "that it is broken...");
    fonts[name] = face;
    fonts[fontPath] = face;
    return *face;
}

void MgrFreetype::addFont(const std::string& fontNameOrPath)
{
    addFont(fontNameOrPath, fontNameOrPath);
}

const FT_Face& MgrFreetype::getFont(const std::string& fontNameOrPath) const
{
    return *fonts.at(fontNameOrPath);
}
} // namespace vvv3d
