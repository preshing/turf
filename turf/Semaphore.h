/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_SEMAPHORE_H
#define TURF_SEMAPHORE_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_SEMAPHORE_PATH)
    // FIXME: Implement Semaphore_CondVar and use it in TURF_PREFER_CPP11/BOOST configs
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_Win32.h"
        #define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_Win32
    #elif TURF_KERNEL_MACH
        #define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_Mach.h"
        #define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_Mach
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_POSIX.h"
        #define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_POSIX
    #else
        #define TURF_IMPL_SEMAPHORE_PATH "*** Unable to select a default Semaphore implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_SEMAPHORE_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_SEMAPHORE_TYPE Semaphore;
}

#endif // TURF_SEMAPHORE_H
