/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_POSIX_H
#define TURF_IMPL_MUTEX_POSIX_H

#include <turf/Core.h>
#include <pthread.h>

namespace turf {

class Mutex_POSIX {
private:
    friend class ConditionVariable_POSIX;
    pthread_mutex_t m_mutex;

public:
    Mutex_POSIX() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE) || defined(__FreeBSD__)
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#else
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
        pthread_mutex_init(&m_mutex, &attr);
    }

    ~Mutex_POSIX() {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock() {
        pthread_mutex_lock(&m_mutex);
    }

    bool tryLock() {
        return !pthread_mutex_trylock(&m_mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_POSIX_H
