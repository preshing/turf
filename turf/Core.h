/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_CORE_H
#define TURF_CORE_H

// clang-format off

#include <turf/c/core.h>
#include <new>  // For placement new

namespace turf {

//---------------------------------------------------
// Integer types
// Turf only supports 32/64-bit targets with flat address spaces
// (Sizes are validated in Core.cpp)
//---------------------------------------------------
namespace intTypes {
    typedef char s8;
    typedef unsigned char u8;
    typedef short s16;
    typedef unsigned short u16;
    typedef int s32;
    typedef unsigned int u32;
    // FIXME: Define s64/u64 on compilers without long long
#if TURF_HAS_LONG_LONG
    typedef long long s64;
    typedef unsigned long long u64;
#endif    
#if TURF_PTR_SIZE == 4
    typedef s32 sptr;
    typedef u32 uptr;
    typedef s32 sreg;
    typedef u32 ureg;
#else
    typedef s64 sptr;
    typedef u64 uptr;
    typedef s64 sreg;
    typedef u64 ureg;
#endif
}
using namespace intTypes;

} // namespace turf

//---------------------------------------------------
// C++ macros
//---------------------------------------------------
#define TURF_CALL_MEMBER(obj, pmf) ((obj).*(pmf))
#if TURF_HAS_STATIC_ASSERT
#define TURF_STATIC_ASSERT(x) static_assert(x, #x)
#else
#define TURF_STATIC_ASSERT(x) typedef char TURF_UNIQUE_VARIABLE(StaticAssert)[(x) ? 1 : -1]
#endif

#if TURF_HAS_NOEXCEPT
#define TURF_NOEXCEPT noexcept
#else
#define TURF_NOEXCEPT
#endif

#if TURF_HAS_CONSTEXPR
#define TURF_CONSTEXPR constexpr
#else
#define TURF_CONSTEXPR
#endif

#if TURF_HAS_OVERRIDE
#define TURF_OVERRIDE override
#else
#define TURF_OVERRIDE
#endif

#if TURF_HAS_MOVE
#include <utility>
#define TURF_MOVE(x) std::move(x)
#else
#define TURF_MOVE(x) (x)
#endif

#endif // TURF_CORE_H
