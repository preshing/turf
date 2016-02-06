/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_THREAD_H
#define TURF_THREAD_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_THREAD_PATH)
    #if TURF_PREFER_CPP11
        #define TURF_IMPL_THREAD_PATH "impl/Thread_CPP11.h"
        #define TURF_IMPL_THREAD_TYPE turf::Thread_CPP11
    #elif TURF_PREFER_BOOST
        #define TURF_IMPL_THREAD_PATH "impl/Thread_Boost.h"
        #define TURF_IMPL_THREAD_TYPE turf::Thread_Boost
    #elif TURF_TARGET_WIN32
        #define TURF_IMPL_THREAD_PATH "impl/Thread_Win32.h"
        #define TURF_IMPL_THREAD_TYPE turf::Thread_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_THREAD_PATH "impl/Thread_POSIX.h"
        #define TURF_IMPL_THREAD_TYPE turf::Thread_POSIX
    #else
        #define TURF_IMPL_THREAD_PATH "*** Unable to select a default Thread implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_THREAD_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_THREAD_TYPE Thread;
}

#endif // TURF_THREAD_H
