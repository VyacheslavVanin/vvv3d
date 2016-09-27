#include "vertexattribute.h"

static const std::map<ATTRIB_LOCATION,std::string> attributesNamesBindings =
{
    {ATTRIB_LOCATION::POSITION, "position"  },
    {ATTRIB_LOCATION::TEXCOORD, "texCoord"  },
    {ATTRIB_LOCATION::NORMAL,   "normal"    },
    {ATTRIB_LOCATION::COLOR,    "color"     },
};

const char *getAttribLocationName(ATTRIB_LOCATION attrib)
{
    auto i = attributesNamesBindings.find( attrib );
    if( i == attributesNamesBindings.end() )
        throw std::logic_error("No such attrib");
    return (*i).second.c_str();
}


void bindAttribLocations(GLuint program)
{
    for(const auto& a: attributesNamesBindings)
        glBindAttribLocation(program, static_cast<GLuint>(a.first), a.second.c_str());
}
