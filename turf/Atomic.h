/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_ATOMIC_H
#define TURF_ATOMIC_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_ATOMIC_PATH)
    // FIXME: Fix atomic_gcc_arm.c, support AArch64, then remove TURF_CPU_ARM/ARM64 checks here:
    #if TURF_PREFER_CPP11 || TURF_CPU_ARM || TURF_CPU_ARM64 || TURF_CPU_POWERPC
        #define TURF_IMPL_ATOMIC_PATH "impl/Atomic_CPP11.h"
        #define TURF_IMPL_ATOMIC_TYPE turf::Atomic_CPP11
    #elif TURF_PREFER_BOOST
        #define TURF_IMPL_ATOMIC_PATH "impl/Atomic_Boost.h"
        #define TURF_IMPL_ATOMIC_TYPE turf::Atomic_Boost
    #else
        #define TURF_IMPL_ATOMIC_PATH "impl/Atomic_Native.h"
        #define TURF_IMPL_ATOMIC_TYPE turf::Atomic_Native
        #if !defined(TURF_C_SYS_ATOMIC_PATH)
            #if TURF_COMPILER_MSVC
                #define TURF_C_SYS_ATOMIC_PATH "turf/c/impl/atomic_msvc.h"
            #elif TURF_COMPILER_GCC && (TURF_CPU_X86 || TURF_CPU_X64)
                #define TURF_C_SYS_ATOMIC_PATH "turf/c/impl/atomic_gcc_x86-64.h"
            #elif TURF_COMPILER_GCC && TURF_CPU_ARM
                #define TURF_C_SYS_ATOMIC_PATH "turf/c/impl/atomic_gcc_arm.h"
            #else
                #define TURF_C_SYS_ATOMIC_PATH "*** Unable to select a default Atomic implementation ***"
            #endif
        #endif
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_ATOMIC_PATH

// Alias it:
namespace turf {
    
template <typename T> class Atomic : public TURF_IMPL_ATOMIC_TYPE<T> {
public:
    Atomic() {
    }
    Atomic(T value) : TURF_IMPL_ATOMIC_TYPE<T>(value) {
    }
    ~Atomic() {
    }
};

} // namespace turf

#endif // TURF_ATOMIC_H
