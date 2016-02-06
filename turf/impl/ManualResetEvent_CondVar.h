/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MANUALRESETEVENT_CONDVAR_H
#define TURF_IMPL_MANUALRESETEVENT_CONDVAR_H

#include <turf/Core.h>
#include <turf/ConditionVariable.h>

namespace turf {

class ManualResetEvent_CondVar {
private:
    turf::Mutex m_mutex;
    turf::ConditionVariable m_condVar;
    bool m_state;

public:
    ManualResetEvent_CondVar(bool initialState = false) : m_state(initialState) {
    }

    ~ManualResetEvent_CondVar() {
    }

    void signal() {
        turf::LockGuard<turf::Mutex> guard(m_mutex);
        m_state = true;
        m_condVar.wakeAll();
    }

    void reset() {
        turf::LockGuard<turf::Mutex> guard(m_mutex);
        m_state = false;
    }

    void wait() {
        turf::LockGuard<turf::Mutex> guard(m_mutex);
        while (!m_state) {
            m_condVar.wait(guard);
        }
    }
};

} // namespace turf

#endif // TURF_IMPL_MANUALRESETEVENT_CONDVAR_H
