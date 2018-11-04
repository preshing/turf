/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_UTIL_H
#define TURF_UTIL_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {
namespace util {


// clang-format off

template <typename T>
struct TruncateTraits;
template <>
struct TruncateTraits<s8> {
    typedef sreg ArgType;
    static const ArgType Min = INT8_MIN;
    static const ArgType Max = INT8_MAX;
};
template <>
struct TruncateTraits<u8> {
    typedef ureg ArgType;
    static const ArgType Min = 0;
    static const ArgType Max = UINT8_MAX;
};
template <>
struct TruncateTraits<s16> {
    typedef sreg ArgType;
    static const ArgType Min = INT16_MIN;
    static const ArgType Max = INT16_MAX;
};
template <>
struct TruncateTraits<u16> {
    typedef ureg ArgType;
    static const ArgType Min = 0;
    static const ArgType Max = UINT16_MAX;
};
template <>
struct TruncateTraits<s32> {
    typedef sreg ArgType;
    static const ArgType Min = INT32_MIN;
    static const ArgType Max = INT32_MAX;
};
template <>
struct TruncateTraits<u32> {
    typedef ureg ArgType;
    static const ArgType Min = 0;
    static const ArgType Max = UINT32_MAX;
};
template <>
struct TruncateTraits<s64> {
    typedef s64 ArgType;
    static const ArgType Min = INT64_MIN;
    static const ArgType Max = INT64_MAX;
};
template <>
struct TruncateTraits<u64> {
    typedef u64 ArgType;
    static const ArgType Min = 0;
    static const ArgType Max = UINT64_MAX;
};

template <typename T>
struct BestFit;
template <>
struct BestFit<s32> {
    typedef u32 Unsigned;
    typedef s32 Signed;
};
template <>
struct BestFit<u32> {
    typedef u32 Unsigned;
    typedef s32 Signed;
};
template <>
struct BestFit<s64> {
    typedef u64 Unsigned;
    typedef s64 Signed;
};
template <>
struct BestFit<u64> {
    typedef u64 Unsigned;
    typedef s64 Signed;
};
#if TURF_LONG_SIZE == 4
    template <>
    struct BestFit<long> {
        typedef u32 Unsigned;
        typedef s32 Signed;
    };
    template <>
    struct BestFit<unsigned long> {
        typedef u32 Unsigned;
        typedef s32 Signed;
    };
#elif TURF_LONG_SIZE == 8
    template <>
    struct BestFit<long> {
        typedef u64 Unsigned;
        typedef s64 Signed;
    };
    template <>
    struct BestFit<unsigned long> {
        typedef u64 Unsigned;
        typedef s64 Signed;
    };
#endif
template <typename T>
struct BestFit<T*> {
    typedef uptr Unsigned;
    typedef sptr Signed;
};

// clang-format on

inline bool isPowerOf2(ureg v) {
    return (v & (v - 1)) == 0;
}

inline ureg alignPowerOf2(ureg v, ureg a) {
    TURF_ASSERT(isPowerOf2(a));
    return (v + a - 1) & ~(a - 1);
}

inline bool isAlignedPowerOf2(ureg v, ureg a) {
    TURF_ASSERT(isPowerOf2(a));
    return (v & (a - 1)) == 0;
}

inline u32 roundUpPowerOf2(u32 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

inline s32 roundUpPowerOf2(s32 v) {
    return (s32) roundUpPowerOf2((u32) v);
}

inline u64 roundUpPowerOf2(u64 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}

inline s64 roundUpPowerOf2(s64 v) {
    return (s64) roundUpPowerOf2((u64) v);
}

inline ureg countSetBits(u64 mask) {
    ureg count = 0;
    while (mask) {
        count += (mask & 1);
        mask >>= 1;
    }
    return count;
}

template <typename T>
inline T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T>
inline T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
inline T truncateChecked(typename TruncateTraits<T>::ArgType src) {
    TURF_ASSERT(src >= TruncateTraits<T>::Min && src <= TruncateTraits<T>::Max);
    return (T) src;
}

// from code.google.com/p/smhasher/wiki/MurmurHash3
inline u32 avalanche(u32 h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

inline u32 deavalanche(u32 h) {
    h ^= h >> 16;
    h *= 0x7ed1b41d;
    h ^= (h ^ (h >> 13)) >> 13;
    h *= 0xa5cb9243;
    h ^= h >> 16;
    return h;
}

// from code.google.com/p/smhasher/wiki/MurmurHash3
inline u64 avalanche(u64 h) {
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53;
    h ^= h >> 33;
    return h;
}

inline u64 deavalanche(u64 h) {
    h ^= h >> 33;
    h *= 0x9cb4b2f8129337db;
    h ^= h >> 33;
    h *= 0x4f74430c22a54005;
    h ^= h >> 33;
    return h;
}

} // namespace util
} // namespace turf

#endif // TURF_UTIL_H
