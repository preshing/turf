/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_RWLOCK_H
#define TURF_RWLOCK_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_RWLOCK_PATH)
    // FIXME: Implement RWLock_CPP11/14, RWLock_Boost
    #if TURF_TARGET_WIN32
        #define TURF_IMPL_RWLOCK_PATH "impl/RWLock_Win32.h"
        #define TURF_IMPL_RWLOCK_TYPE turf::RWLock_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_RWLOCK_PATH "impl/RWLock_POSIX.h"
        #define TURF_IMPL_RWLOCK_TYPE turf::RWLock_POSIX
    #else
        #define TURF_IMPL_RWLOCK_PATH "*** Unable to select a default RWLock implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_RWLOCK_PATH

// Alias it:
namespace turf {

typedef TURF_IMPL_RWLOCK_TYPE RWLock;

//---------------------------------------------------------
// SharedLockGuard
//---------------------------------------------------------
template <typename LockType> class SharedLockGuard {
private:
    LockType& m_lock;

public:
    SharedLockGuard(LockType& lock) : m_lock(lock) {
        m_lock.lockShared();
    }

    ~SharedLockGuard() {
        m_lock.unlockShared();
    }
};

//---------------------------------------------------------
// ExclusiveLockGuard
//---------------------------------------------------------
template <typename LockType> class ExclusiveLockGuard {
private:
    LockType& m_lock;

public:
    ExclusiveLockGuard(LockType& lock) : m_lock(lock) {
        m_lock.lockExclusive();
    }

    ~ExclusiveLockGuard() {
        m_lock.unlockExclusive();
    }
};

} // namespace turf

#endif // TURF_RWLOCK_H
