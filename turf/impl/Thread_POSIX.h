/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_THREAD_POSIX_H
#define TURF_IMPL_THREAD_POSIX_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

namespace turf {

#define TURF_THREAD_STARTCALL

class Thread_POSIX {
private:
    pthread_t m_handle;
    bool m_attached;

public:
    typedef void* ReturnType;
    typedef void* (*StartRoutine)(void*);

    Thread_POSIX() : m_attached(false) {
        memset(&m_handle, 0, sizeof(m_handle));
    }

    Thread_POSIX(StartRoutine startRoutine, void* arg = NULL) {
        pthread_create(&m_handle, NULL, startRoutine, arg);
        m_attached = true;
    }

    ~Thread_POSIX() {
        if (m_attached)
            pthread_detach(m_handle);
    }

    void run(StartRoutine startRoutine, void* arg = NULL) {
        TURF_ASSERT(!m_attached);
        pthread_create(&m_handle, NULL, startRoutine, arg);
        m_attached = true;
    }

    void join() {
        TURF_ASSERT(m_attached);
        void* retVal;
        pthread_join(m_handle, &retVal);
        memset(&m_handle, 0, sizeof(m_handle));
        m_attached = false;
    }

#if !TURF_TARGET_MINGW
    static void sleepMillis(ureg millis) {
        timespec ts;
        ts.tv_sec = millis / 1000;
        ts.tv_nsec = (millis % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
#endif
};

} // namespace turf

#endif // TURF_IMPL_THREAD_POSIX_H
