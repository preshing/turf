/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_RWLOCK_WIN32_H
#define TURF_IMPL_RWLOCK_WIN32_H

#include <turf/Core.h>

namespace turf {

class RWLock_Win32 {
private:
    SRWLOCK m_rwLock;

public:
    RWLock_Win32() {
        InitializeSRWLock(&m_rwLock);
    }

    ~RWLock_Win32() {
        // SRW locks do not need to be explicitly destroyed.
    }

    void lockExclusive() {
        AcquireSRWLockExclusive(&m_rwLock);
    }

    void unlockExclusive() {
        ReleaseSRWLockExclusive(&m_rwLock);
    }

    void lockShared() {
        AcquireSRWLockShared(&m_rwLock);
    }

    void unlockShared() {
        ReleaseSRWLockShared(&m_rwLock);
    }
};

} // namespace turf

#endif // TURF_IMPL_RWLOCK_WIN32_H
