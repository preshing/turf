/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_TID_H
#define TURF_TID_H

// Turf TIDs are integers (of some size) that are mainly used for logging and debugging.
// Ideally, Turf TIDs match the thread IDs shown in the debugger.

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_TID_PATH)
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_TID_PATH "impl/TID_Win32.h"
        #define TURF_IMPL_TID_TYPE turf::TID_Win32
    #elif TURF_KERNEL_MACH
        #define TURF_IMPL_TID_PATH "impl/TID_Mach.h"
        #define TURF_IMPL_TID_TYPE turf::TID_Mach
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_TID_PATH "impl/TID_POSIX.h"
        #define TURF_IMPL_TID_TYPE turf::TID_POSIX
    #else
        #define TURF_IMPL_TID_PATH "*** Unable to select a default TID implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_TID_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_TID_TYPE TID;
}

#endif // TURF_TID_H
