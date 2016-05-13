/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CONDITIONVARIABLE_WIN32_H
#define TURF_IMPL_CONDITIONVARIABLE_WIN32_H

#include <turf/Core.h>
#include <turf/impl/Mutex_Win32.h>

namespace turf {

class ConditionVariable_Win32 {
private:
    CONDITION_VARIABLE m_condVar;

public:
    ConditionVariable_Win32() {
        InitializeConditionVariable(&m_condVar);
    }

    void wait(LockGuard<Mutex_Win32>& guard) {
        SleepConditionVariableCS(&m_condVar, &guard.getMutex().m_mutex, INFINITE);
    }

    void timedWait(LockGuard<Mutex_Win32>& guard, ureg waitMillis) {
        if (waitMillis > 0)
            SleepConditionVariableCS(&m_condVar, &guard.getMutex().m_mutex, waitMillis);
    }

    void wakeOne() {
        WakeConditionVariable(&m_condVar);
    }

    void wakeAll() {
        WakeAllConditionVariable(&m_condVar);
    }
};

} // namespace turf

#endif // TURF_IMPL_CONDITIONVARIABLE_WIN32_H
