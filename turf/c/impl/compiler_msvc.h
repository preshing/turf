/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_COMPILER_MSVC_H
#define TURF_C_IMPL_COMPILER_MSVC_H

#include <intrin.h>

//-------------------------------------
//  Alignment
//-------------------------------------
#define TURF_DECL_ALIGNED(declaration, amt) __declspec(align(amt)) declaration

//-------------------------------------
//  Inlining
//-------------------------------------
#ifdef __cplusplus
#define TURF_C_INLINE inline
#else
#define TURF_C_INLINE __inline
#endif

#define TURF_FORCE_INLINE __forceinline
#define TURF_NO_INLINE __declspec(noinline)

//-------------------------------------
//  Thread local
//-------------------------------------
#define TURF_THREAD_LOCAL __declspec(thread)

//-------------------------------------
//  Debug break
//-------------------------------------
#define TURF_DEBUG_BREAK() __debugbreak()

TURF_C_INLINE void turf_yieldHWThread() {
    YieldProcessor();
}

#endif // TURF_C_IMPL_COMPILER_MSVC_H
