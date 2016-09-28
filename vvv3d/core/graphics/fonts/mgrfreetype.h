#ifndef FREETYPECONTAINER
#define FREETYPECONTAINER
#include <map>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class MgrFreetype
{
public:
    MgrFreetype()
    {
        if(FT_Init_FreeType(&library)) throw std::logic_error("Error init freetype");
    }

    ~MgrFreetype() { FT_Done_FreeType(library); }

    FT_Face addFont(const std::string& name, const std::string& fontPath)
    {
        FT_Face     face;
        const FT_Error error = FT_New_Face( library, fontPath.c_str(), 0, &face );
        if ( error == FT_Err_Unknown_File_Format )
          throw std::logic_error("the font file could be opened and read, but it appears"
                                 " that its font format is unsupported");
        else if ( error )
          throw std::logic_error("the font file could not be opened or read, or that it is broken...");
        fonts[name]=face;
        return face;
    }

    void addFont(const std::string& fontNameOrPath)
    {
        addFont(fontNameOrPath, fontNameOrPath);
    }

    FT_Face getFont(const std::string& fontNameOrPath) const
    { return fonts.at(fontNameOrPath); }

private:
    FT_Library  library;
    std::map<std::string, FT_Face> fonts;
};


#endif // FREETYPECONTAINER

