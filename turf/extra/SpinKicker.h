/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_SPINKICKER_H
#define TURF_EXTRA_SPINKICKER_H

#include <turf/Core.h>
#include <turf/Thread.h>
#include <turf/Atomic.h>

namespace turf {
namespace extra {

class SpinKicker {
private:
    turf::Atomic<ureg> m_kickVar;

public:
    SpinKicker() : m_kickVar(0) {
    }

    void waitForKick() {
        // Increment shared counter to indicate this thread is ready and
        // busy-waiting.
        m_kickVar.fetchAdd(1, turf::Release);
        // Busy-wait on kick variable until there's a kick from main thread.
        while (m_kickVar.load(turf::Acquire) > 0) {
        }
    }

    void kick(ureg count) {
        // Wait until all workers are ready and busy-waiting on the kick variable.
        while (m_kickVar.load(turf::Acquire) < count) {
        }
        // Kick.
        m_kickVar.store(0, turf::Release);
    }
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_SPINKICKER_H
