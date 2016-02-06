/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_MUTEX_H
#define TURF_MUTEX_H

#include <turf/Core.h>

namespace turf {
template <typename LockType>
class LockGuard;
} // namespace turf

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_MUTEX_PATH)
    #if TURF_PREFER_CPP11
        #define TURF_IMPL_MUTEX_PATH "impl/Mutex_CPP11.h"
        #define TURF_IMPL_MUTEX_TYPE turf::Mutex_CPP11
    #elif TURF_PREFER_BOOST
        #define TURF_IMPL_MUTEX_PATH "impl/Mutex_Boost.h"
        #define TURF_IMPL_MUTEX_TYPE turf::Mutex_Boost
    #elif TURF_TARGET_WIN32
        #define TURF_IMPL_MUTEX_PATH "impl/Mutex_Win32.h"
        #define TURF_IMPL_MUTEX_TYPE turf::Mutex_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_MUTEX_PATH "impl/Mutex_POSIX.h"
        #define TURF_IMPL_MUTEX_TYPE turf::Mutex_POSIX
    #else
        #define TURF_IMPL_MUTEX_PATH "*** Unable to select a default Mutex implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_MUTEX_PATH

// Alias it:
namespace turf {

typedef TURF_IMPL_MUTEX_TYPE Mutex;

//---------------------------------------------------------
// Generic LockGuard
//---------------------------------------------------------
template <typename LockType> class LockGuard {
private:
    LockType& m_lock;

public:
    LockGuard(LockType& lock) : m_lock(lock) {
        m_lock.lock();
    }
    ~LockGuard() {
        m_lock.unlock();
    }
    LockType& getMutex() {
        return m_lock;
    }
};

} // namespace turf

#endif // TURF_MUTEX_H
