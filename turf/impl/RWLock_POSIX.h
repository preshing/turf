/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_RWLOCK_POSIX_H
#define TURF_IMPL_RWLOCK_POSIX_H

#include <turf/Core.h>
#include <pthread.h>

namespace turf {

class RWLock_POSIX {
private:
    pthread_rwlock_t m_rwLock;

public:
    RWLock_POSIX() {
        pthread_rwlock_init(&m_rwLock, NULL);
    }

    ~RWLock_POSIX() {
        pthread_rwlock_destroy(&m_rwLock);
    }

    void lockExclusive() {
        pthread_rwlock_wrlock(&m_rwLock);
    }

    void unlockExclusive() {
        pthread_rwlock_unlock(&m_rwLock);
    }

    void lockShared() {
        pthread_rwlock_rdlock(&m_rwLock);
    }

    void unlockShared() {
        pthread_rwlock_unlock(&m_rwLock);
    }
};

} // namespace turf

#endif // TURF_IMPL_RWLOCK_POSIX_H
