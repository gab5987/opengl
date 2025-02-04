#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <string>

namespace util
{
inline std::string randstr(std::string::size_type length)
{
    static constexpr const auto &CHRS = "0123456789"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rgen{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type>
        pick(0, sizeof(CHRS) - 2);

    std::string str;

    str.reserve(length);

    while (length-- > 0)
    {
        str += CHRS[pick(rgen)];
    }

    return str;
}
}; // namespace util

#endif
