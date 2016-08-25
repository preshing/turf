/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_CPUTIMER_H
#define TURF_CPUTIMER_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_CPUTIMER_PATH)
    #if TURF_PREFER_CPP11
        #define TURF_IMPL_CPUTIMER_PATH "impl/CPUTimer_CPP11.h"
        #define TURF_IMPL_CPUTIMER_TYPE turf::CPUTimer_CPP11
        // FIXME: Add CPUTimer_Boost
    #elif TURF_TARGET_WIN32 || TURF_TARGET_MINGW
        #define TURF_IMPL_CPUTIMER_PATH "impl/CPUTimer_Win32.h"
        #define TURF_IMPL_CPUTIMER_TYPE turf::CPUTimer_Win32
    #elif TURF_KERNEL_MACH
        #define TURF_IMPL_CPUTIMER_PATH "impl/CPUTimer_Mach.h"
        #define TURF_IMPL_CPUTIMER_TYPE turf::CPUTimer_Mach
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_CPUTIMER_PATH "impl/CPUTimer_POSIX.h"
        #define TURF_IMPL_CPUTIMER_TYPE turf::CPUTimer_POSIX
    #else
        #define TURF_IMPL_CPUTIMER_PATH "*** Unable to select a default CPUTimer implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_CPUTIMER_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_CPUTIMER_TYPE CPUTimer;
}

#endif // TURF_CPUTIMER_H
