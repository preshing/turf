/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_THREADLOCAL_H
#define TURF_THREADLOCAL_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_THREADLOCAL_PATH)
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_THREADLOCAL_PATH "impl/ThreadLocal_Win32.h"
        #define TURF_IMPL_THREADLOCAL_TYPE turf::ThreadLocal_Win32
        #define TURF_IMPL_THREADLOCALSCOPE_TYPE turf::ThreadLocalScope_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_THREADLOCAL_PATH "impl/ThreadLocal_POSIX.h"
        #define TURF_IMPL_THREADLOCAL_TYPE turf::ThreadLocal_POSIX
        #define TURF_IMPL_THREADLOCALSCOPE_TYPE turf::ThreadLocalScope_POSIX
    #else
        #define TURF_IMPL_THREADLOCAL_PATH "*** Unable to select a default ThreadLocal implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_THREADLOCAL_PATH

// Alias it:
namespace turf {

template<typename T>
class ThreadLocal : public TURF_IMPL_THREADLOCAL_TYPE<T> {
};

template<typename T>
class ThreadLocalScope : public TURF_IMPL_THREADLOCALSCOPE_TYPE<T> {
public:
    ThreadLocalScope(ThreadLocal<T>& ptr, T value) : TURF_IMPL_THREADLOCALSCOPE_TYPE<T>(ptr, value) {
    }
};

} // namespace turf

#endif // TURF_THREADLOCAL_H
