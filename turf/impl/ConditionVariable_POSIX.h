/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CONDITIONVARIABLE_POSIX_H
#define TURF_IMPL_CONDITIONVARIABLE_POSIX_H

#include <turf/Core.h>
#include <turf/impl/Mutex_POSIX.h>
#include <time.h>

namespace turf {

class ConditionVariable_POSIX {
private:
    pthread_cond_t m_condVar;

public:
    ConditionVariable_POSIX() {
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        pthread_cond_init(&m_condVar, &attr);
    }

    ~ConditionVariable_POSIX() {
        pthread_cond_destroy(&m_condVar);
    }

    void wait(LockGuard<Mutex_POSIX>& guard) {
        pthread_cond_wait(&m_condVar, &guard.getMutex().m_mutex);
    }

#if !TURF_TARGET_MINGW
    void timedWait(LockGuard<Mutex_POSIX>& guard, ureg waitMillis) {
        if (waitMillis > 0) {
#if TURF_TARGET_APPLE
            struct timespec ts;
            ts.tv_sec += waitMillis / 1000;
            ts.tv_nsec += (waitMillis % 1000) * 1000000;
            pthread_cond_timedwait_relative_np(&m_condVar, &guard.getMutex().m_mutex, &ts);
#else
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += waitMillis / 1000;
            ts.tv_nsec += (waitMillis % 1000) * 1000000;
            if (ts.tv_nsec >= 1000000000) {
                ts.tv_nsec -= 1000000000;
                ts.tv_sec++;
            }
            pthread_cond_timedwait(&m_condVar, &guard.getMutex().m_mutex, &ts);
#endif
        }
    }
#endif

    void wakeOne() {
        pthread_cond_signal(&m_condVar);
    }

    void wakeAll() {
        pthread_cond_broadcast(&m_condVar);
    }
};

} // namespace turf

#endif // TURF_IMPL_CONDITIONVARIABLE_POSIX_H
