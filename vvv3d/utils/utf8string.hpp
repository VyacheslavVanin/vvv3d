#ifndef VVV_UTF8STRING_HPP
#define VVV_UTF8STRING_HPP
#include <algorithm>
#include <iostream>
#include <string>

namespace vvv {
inline std::string getFirstChar(std::string& str)
{
    uint8_t b0 = str[0];
    if (!(b0 >> 7)) {
        str = str.substr(1);
        return std::string() + (char)b0;
    }

    if ((b0 >> 5) == 6) {
        std::string ret = std::string(str.begin(), str.begin() + 2);
        str = str.substr(2);
        return ret;
    }

    if ((b0 >> 4) == 14) {
        std::string ret = std::string(str.begin(), str.begin() + 3);
        str = str.substr(3);
        return ret;
    }

    if ((b0 >> 3) == 30) {
        std::string ret = std::string(str.begin(), str.begin() + 4);
        str = str.substr(4);
        return ret;
    }

    throw std::invalid_argument("input string is not utf8 string");
}

inline std::string getLastChar(std::string& str)
{
    const auto& it = std::find_if(str.rbegin(), str.rend(),
                                  [](uint8_t c) { return (c >> 6) != 0x2; });
    const auto distance = std::distance(str.rbegin(), it + 1);
    const auto pos = str.size() - distance;
    std::string ret = str.substr(pos);
    str.resize(pos);
    return ret;
}
} // namespace vvv

#endif
