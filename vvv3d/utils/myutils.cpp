#include "myutils.hpp"

#include <fstream>
#include <map>
#include <stdexcept>
#include <streambuf>
#include <string>

namespace vvv3d {

std::string loadFileToString(const char* filename)
{
    std::string ret;
    loadFileToString(filename, ret);
    return ret;
}

void loadFileToString(const char* filename, std::string& out)
{
    using namespace std;
    ifstream f;
    f.exceptions(ifstream::failbit | ifstream::badbit);
    f.open(filename);

    f.seekg(0, std::ios::end);
    out.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    out.assign(istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}
} // namespace vvv3d
