/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_THREAD_CPP11_H
#define TURF_IMPL_THREAD_CPP11_H

#include <turf/Core.h>
#include <thread>
#include <chrono>

namespace turf {

#define TURF_THREAD_STARTCALL

class Thread_CPP11 {
protected:
    std::thread m_thread;

public:
    typedef void* ReturnType;
    typedef void* (*StartRoutine)(void*);

    Thread_CPP11() {
    }

    Thread_CPP11(StartRoutine startRoutine, void* arg = NULL) : m_thread(startRoutine, arg) {
    }

    ~Thread_CPP11() {
        if (m_thread.joinable())
            m_thread.detach();
    }

    void join() {
        m_thread.join();
    }

    void run(StartRoutine startRoutine, void* arg = NULL) {
        if (m_thread.joinable())
            m_thread.detach();
        m_thread = std::thread(startRoutine, arg);
    }

    static void sleepMillis(ureg millis) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }
};

} // namespace turf

#endif // TURF_IMPL_THREAD_CPP11_H
