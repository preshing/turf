/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_SPINLOCK_H
#define TURF_IMPL_MUTEX_SPINLOCK_H

#include <turf/Core.h>
#include <turf/Atomic.h>

namespace turf {

// This class is meant to be zero-initialized at global scope.
class Mutex_SpinLock {
private:
    Atomic<u32> m_spinLock;

public:
    void initialize() {
        m_spinLock.storeNonatomic(0);
    }

    void lock() {
        for (;;) {
            u32 expected = 0;
            if (m_spinLock.compareExchangeStrong(expected, 1, turf::Acquire))
                break;
            // FIXME: Implement reusable AdaptiveBackoff class and apply it here
        }
    }

    void unlock() {
        m_spinLock.store(0, turf::Release);
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_SPINLOCK_H
