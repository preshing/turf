/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_LAZYINIT_H
#define TURF_IMPL_MUTEX_LAZYINIT_H

#include <turf/Core.h>
#include <turf/Atomic.h>
#include <turf/Mutex.h>
#include <turf/Assert.h>
#include <memory.h>

namespace turf {

// A mutex with no constructor that works when zero-init at global scope.
class Mutex_LazyInit {
private:
    Atomic<bool> m_initFlag;
    Atomic<bool> m_spinLock;
    u8 m_buffer[sizeof(Mutex)];

    Mutex& getMutex() {
        return *(Mutex*) m_buffer;
    }

    void lazyInit() {
        // We use the thread-safe DCLI pattern via spinlock in case threads are spawned
        // during static initialization of global C++ objects. In that case, any of them
        // could call lazyInit().
        while (m_spinLock.compareExchange(false, true, turf::Acquire)) {
            // FIXME: Implement reusable AdaptiveBackoff class and apply it here
        }
        if (!m_initFlag.loadNonatomic()) {
            new (&getMutex()) Mutex;
            m_initFlag.store(true, turf::Release);
        }
        m_spinLock.store(false, turf::Release);
    }

public:
    // Manual initialization is needed if not created at global scope:
    void zeroInit() {
        memset(this, 0, sizeof(*this));
    }

    // There should be no threads racing to lock when the destructor is called.
    // It's valid to attempt to lock after the destructor, though.
    // This permits Mutex_LazyInit to be used at global scope where destructors are called in an arbitrary order.
    ~Mutex_LazyInit() {
        if (m_initFlag.loadNonatomic()) {
            getMutex().Mutex::~Mutex();
            zeroInit();
        }
    }

    void lock() {
        if (!m_initFlag.load(turf::Acquire))
            lazyInit();
        getMutex().lock();
    }

    void unlock() {
        TURF_ASSERT(m_initFlag.loadNonatomic());
        getMutex().unlock();
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_LAZYINIT_H
