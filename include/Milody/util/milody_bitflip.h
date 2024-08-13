#ifndef MILODY_BITFLIP_H
#define MILODY_BITFLIP_H

#include <cstdint>
#include <type_traits>

#if WIN32
#include <cstdlib>
#endif

namespace milody::util::bitflip {
#if defined(__GNUC__) && __GNUC__ >= 2

static inline uint16_t bswap16(uint16_t x) {
    return __builtin_bswap16(x);
}

static inline uint32_t bswap32(uint32_t x) {
    return __builtin_bswap32(x);
}

static inline uint64_t bswap64(uint64_t x) {
    return __builtin_bswap64(x);
}

#elif defined(_MSC_VER)
static inline uint16_t bswap16(uint16_t x) {
    return _byteswap_ushort(x);
}

static inline uint32_t bswap32(uint32_t x) {
    return _byteswap_ulong(x);
}

static inline uint64_t bswap64(uint64_t x) {
    return _byteswap_uint64(x);
}
#else
static inline uint16_t bswap16(uint16_t x) {
    return (x >> 8) | (x << 8);
}

static inline uint32_t bswap32(uint32_t x) {
    x = (x >> 16) | (x << 16);
    x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
    return x;
}

static inline uint64_t bswap64(uint64_t x) {
    return bswap32(x >> 32) | (((uint64_t) bswap32(x)) << 32);
}
#endif
template <typename T>
concept UInt = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t> ||
               std::is_same_v<T, uint64_t>;

template <UInt T>
static inline T bswap(T x) {
    if constexpr (std::is_same_v<T, uint16_t>)
        return bswap16(x);
    else if constexpr (std::is_same_v<T, uint32_t>)
        return bswap32(x);
    else if constexpr (std::is_same_v<T, uint64_t>)
        return bswap64(x);
    else
        return x;
}
} // namespace milody::util::bitflip

#endif //MILODY_MILODY_BITFLIP_H
