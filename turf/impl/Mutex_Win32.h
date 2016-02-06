/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_WIN32_H
#define TURF_IMPL_MUTEX_WIN32_H

#include <turf/Core.h>

namespace turf {

class Mutex_Win32 {
private:
    friend class ConditionVariable_Win32;
    CRITICAL_SECTION m_mutex;

public:
    Mutex_Win32() {
        InitializeCriticalSection(&m_mutex);
    }

    ~Mutex_Win32() {
        DeleteCriticalSection(&m_mutex);
    }

    void lock() {
        EnterCriticalSection(&m_mutex);
    }

    bool tryLock() {
        return !!TryEnterCriticalSection(&m_mutex);
    }

    void unlock() {
        LeaveCriticalSection(&m_mutex);
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_WIN32_H
