/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_AFFINITY_H
#define TURF_AFFINITY_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_AFFINITY_PATH)
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_AFFINITY_PATH "impl/Affinity_Win32.h"
        #define TURF_IMPL_AFFINITY_TYPE turf::Affinity_Win32
    #elif TURF_KERNEL_LINUX
        #define TURF_IMPL_AFFINITY_PATH "impl/Affinity_Linux.h"
        #define TURF_IMPL_AFFINITY_TYPE turf::Affinity_Linux
    #elif TURF_KERNEL_FREEBSD
        #define TURF_IMPL_AFFINITY_PATH "impl/Affinity_FreeBSD.h"
        #define TURF_IMPL_AFFINITY_TYPE turf::Affinity_FreeBSD
    #elif TURF_KERNEL_MACH
        #define TURF_IMPL_AFFINITY_PATH "impl/Affinity_Mach.h"
        #define TURF_IMPL_AFFINITY_TYPE turf::Affinity_Mach
    #elif TURF_TARGET_MINGW
        // FIXME: Is there an API to detect CPU topology on MinGW?
        #define TURF_IMPL_AFFINITY_PATH "impl/Affinity_Null.h"
        #define TURF_IMPL_AFFINITY_TYPE turf::Affinity_Null
    #else
        #define TURF_IMPL_AFFINITY_PATH "*** Unable to select a default Affinity implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_AFFINITY_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_AFFINITY_TYPE Affinity;
}

#endif // TURF_AFFINITY_H
