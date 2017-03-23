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
    #if TURF_PREFER_BOOST
        #define TURF_IMPL_THREADLOCAL_PATH "impl/ThreadLocal_Boost.h"
        #define TURF_IMPL_THREADLOCAL_TYPE turf::ThreadLocal_Boost
    #elif TURF_TARGET_WIN32
        #define TURF_IMPL_THREADLOCAL_PATH "impl/ThreadLocal_Win32.h"
        #define TURF_IMPL_THREADLOCAL_TYPE turf::ThreadLocal_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_THREADLOCAL_PATH "impl/ThreadLocal_POSIX.h"
        #define TURF_IMPL_THREADLOCAL_TYPE turf::ThreadLocal_POSIX
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
public:
    void operator=(T value) {
        set(value);
    }
};

template<typename T>
class ThreadLocalScope {
private:
    ThreadLocal<T>& m_ptr;
    T m_oldValue;

public:
    ThreadLocalScope(ThreadLocal<T>& ptr, T value) : m_ptr(ptr) {
        m_oldValue = m_ptr;
        m_ptr = value;
    }

    ~ThreadLocalScope() {
        m_ptr = m_oldValue;
    }
};

} // namespace turf

#endif // TURF_THREADLOCAL_H
