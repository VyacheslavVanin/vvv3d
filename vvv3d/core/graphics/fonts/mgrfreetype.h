#ifndef FREETYPECONTAINER
#define FREETYPECONTAINER
#include <map>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class MgrFreetype
{
public:
    MgrFreetype();
    ~MgrFreetype();

    MgrFreetype(const MgrFreetype&) = delete;
    MgrFreetype& operator=(const MgrFreetype&) = delete;

    FT_Face addFont(const std::string& name, const std::string& fontPath);
    void addFont(const std::string& fontNameOrPath);
    FT_Face getFont(const std::string& fontNameOrPath) const;

private:
    FT_Library  library;
    std::map<std::string, FT_Face> fonts;
};


#endif // FREETYPECONTAINER

