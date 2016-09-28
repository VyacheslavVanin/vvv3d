#include "myutils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <map>

std::string loadFileToString(const char *filename)
{
    std::string ret;
    loadFileToString(filename,ret);
    return ret;
}

void loadFileToString(const char *filename, std::string &out)
{
    using namespace std;
    ifstream f;
    f.exceptions( ifstream::failbit | ifstream::badbit);
    f.open(filename);

    f.seekg(0, std::ios::end);
    out.reserve( f.tellg() );
    f.seekg(0, std::ios::beg);
    out.assign(istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}
