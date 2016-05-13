/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CONDITIONVARIABLE_CPP11_H
#define TURF_IMPL_CONDITIONVARIABLE_CPP11_H

#include <turf/Core.h>
#include <turf/impl/Mutex_CPP11.h>
#include <condition_variable>

namespace turf {

class ConditionVariable_CPP11 {
private:
    std::condition_variable m_condVar;

public:
    void wait(turf::LockGuard<Mutex_CPP11>& guard) {
        m_condVar.wait(guard);
    }

    void timedWait(turf::LockGuard<Mutex_CPP11>& guard, ureg waitMillis) {
        if (waitMillis > 0)
            m_condVar.wait_for(guard, std::chrono::milliseconds(waitMillis));
    }

    void wakeOne() {
        m_condVar.notify_one();
    }

    void wakeAll() {
        m_condVar.notify_all();
    }
};

} // namespace turf

#endif // TURF_IMPL_CONDITIONVARIABLE_CPP11_H
