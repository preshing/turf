/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_MANUALRESETEVENT_H
#define TURF_MANUALRESETEVENT_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_MANUALRESETEVENT_PATH)
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_MANUALRESETEVENT_PATH "impl/ManualResetEvent_Win32.h"
        #define TURF_IMPL_MANUALRESETEVENT_TYPE turf::ManualResetEvent_Win32
    #else
        #define TURF_IMPL_MANUALRESETEVENT_PATH "impl/ManualResetEvent_CondVar.h"
        #define TURF_IMPL_MANUALRESETEVENT_TYPE turf::ManualResetEvent_CondVar
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_MANUALRESETEVENT_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_MANUALRESETEVENT_TYPE ManualResetEvent;
}

#endif // TURF_MANUALRESETEVENT_H
