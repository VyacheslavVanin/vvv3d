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

class utf8string {
public:
    template <typename Iterator>
    class iterator_impl {
    public:
        inline std::string operator*();
        inline iterator_impl& operator++();
        inline iterator_impl& operator--();
        inline bool operator!=(const iterator_impl& other) const;
        inline bool operator<(const iterator_impl& other) const;

    private:
        Iterator it;
        friend utf8string;
    };

    using iterator = iterator_impl<std::string::iterator>;
    using const_iterator = iterator_impl<std::string::const_iterator>;
    using reverse_iterator = iterator_impl<std::string::reverse_iterator>;
    using const_reverse_iterator = iterator_impl<std::string::const_reverse_iterator>;

    utf8string() = default;
    utf8string(const utf8string&) = default;
    utf8string(utf8string&&) = default;
    utf8string& operator=(const utf8string&) = default;
    utf8string& operator=(utf8string&&) = default;

    utf8string(const std::string& other) : data(other) {}
    utf8string(std::string&& other) : data(std::move(other)) {}
    utf8string& operator=(const std::string& other)
    {
        data = other;
        return *this;
    }
    utf8string& operator=(std::string&& other)
    {
        data = std::move(other);
        return *this;
    }

    bool operator==(const utf8string& other) const { return data == other.data; }
    bool operator!=(const utf8string& other) const { return data != other.data; }
    bool operator<(const utf8string& other) const { return data < other.data; }
    bool operator>(const utf8string& other) const { return data < other.data; }

    const std::string& to_std_string() const { return data; }
    std::string& to_std_string() { return data; }
    const char* c_str() const { return data.c_str(); }

    iterator begin()
    {
        iterator ret;
        ret.it = data.begin();
        return ret;
    }
    const_iterator begin() const
    {
        const_iterator ret;
        ret.it = data.cbegin();
        return ret;
    }
    iterator end()
    {
        iterator ret;
        ret.it = data.end();
        return ret;
    }
    const_iterator end() const
    {
        const_iterator ret;
        ret.it = data.cend();
        return ret;
    }
    reverse_iterator rbegin()
    {
        reverse_iterator ret;
        ret.it = data.rbegin();
        return ret;
    }
    const_reverse_iterator rbegin() const
    {
        const_reverse_iterator ret;
        ret.it = data.crbegin();
        return ret;
    }
    reverse_iterator rend()
    {
        reverse_iterator ret;
        ret.it = data.rend();
        return ret;
    }
    const_reverse_iterator rend() const
    {
        const_reverse_iterator ret;
        ret.it = data.crend();
        return ret;
    }

private:
    std::string data;
};

template <typename Iterator>
bool utf8string::iterator_impl<Iterator>::
operator!=(const iterator_impl<Iterator>& other) const
{
    return this->it != other.it;
}

template <typename Iterator>
bool utf8string::iterator_impl<Iterator>::
operator<(const iterator_impl<Iterator>& other) const
{
    return this->it < other.it;
}

template <typename Iterator>
std::string utf8string::iterator_impl<Iterator>::operator*()
{
    auto next = *this;
    ++next;
    return std::string(this->it, next.it);
}

template <typename Iterator>
utf8string::iterator_impl<Iterator>& utf8string::iterator_impl<Iterator>::
operator++()
{
    uint8_t b0 = *it;
    if (!(b0 >> 7))
        ++it;
    else if ((b0 >> 5) == 6)
        it += 2;
    else if ((b0 >> 4) == 14)
        it += 3;
    else if ((b0 >> 3) == 30)
        it += 4;
    return *this;
}

template <>
utf8string::iterator_impl<std::string::reverse_iterator>& utf8string::iterator_impl<std::string::reverse_iterator>::
operator++()
{
    while (((uint8_t)*(it++) >> 6) == 0x02)
        ;
    return *this;
}

template <>
std::string utf8string::iterator_impl<std::string::reverse_iterator>::operator*()
{
    auto next = *this;
    ++next;
    return std::string(next.it.base(), it.base());
}
}

#endif
