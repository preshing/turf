/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_COMPILER_GCC_H
#define TURF_C_IMPL_COMPILER_GCC_H

//-------------------------------------
//  Alignment
//-------------------------------------
// Note: May not work on local variables.
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24691
#define TURF_DECL_ALIGNED(declaration, amt) declaration __attribute__((aligned(amt)))

//-------------------------------------
//  Inlining
//-------------------------------------
#define TURF_C_INLINE static inline
#define TURF_FORCE_INLINE inline __attribute__((always_inline))
#define TURF_NO_INLINE __attribute__((noinline))

//-------------------------------------
//  Thread local
//-------------------------------------
#define TURF_THREAD_LOCAL __thread

//-------------------------------------
//  CPU intrinsics
//-------------------------------------
TURF_C_INLINE void turf_yieldHWThread() {
#if MINT_CPU_X86 || MINT_CPU_X64
    // Only implemented on x86/64
    asm volatile("pause");
#endif
}

#define TURF_DEBUG_BREAK() __builtin_trap()

#endif // TURF_C_IMPL_COMPILER_GCC_H
