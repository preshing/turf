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

namespace turf {
namespace util {

template <typename T>
struct BestFit;

// clang-format off

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
template <class T>
struct BestFit<T*> {
    typedef uptr Unsigned;
    typedef sptr Signed;
};

// clang-format on

inline ureg align(ureg v, ureg a) {
    return (v + a - 1) & ~(a - 1);
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

inline bool isPowerOf2(ureg v) {
    return (v & (v - 1)) == 0;
}

inline ureg countSetBits(u64 mask) {
    ureg count = 0;
    while (mask) {
        count += (mask & 1);
        mask >>= 1;
    }
    return count;
}

template <class T>
T min(T a, T b) {
    return a < b ? a : b;
}

template <class T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <class T>
void swap(T& a, T& b) {
    T t = TURF_MOVE(a);
    a = TURF_MOVE(b);
    b = TURF_MOVE(t);
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
