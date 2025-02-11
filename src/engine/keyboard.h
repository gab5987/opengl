#ifndef ENGINE_KEYBOARD_H
#define ENGINE_KEYBOARD_H

#include <cstdint>

namespace engine
{
enum class key : uint32_t
{
    none  = 0x00,
    front = 0x01,
    back  = 0x02,
    left  = 0x04,
    right = 0x08,
};

inline int operator&(const uint32_t k, key rhs)
{
    return static_cast<uint32_t>(k) & static_cast<uint32_t>(rhs);
}
}; // namespace engine

#endif