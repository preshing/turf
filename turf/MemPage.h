/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_MEMPAGE_H
#define TURF_MEMPAGE_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_MEMPAGE_PATH)
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_MEMPAGE_PATH "impl/MemPage_Win32.h"
        #define TURF_IMPL_MEMPAGE_TYPE turf::MemPage_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_MEMPAGE_PATH "impl/MemPage_POSIX.h"
        #define TURF_IMPL_MEMPAGE_TYPE turf::MemPage_POSIX
    #else
        #define TURF_IMPL_MEMPAGE_PATH "*** Unable to select a default MemPage implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_MEMPAGE_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_MEMPAGE_TYPE MemPage;
}

#endif // TURF_MEMPAGE_H
