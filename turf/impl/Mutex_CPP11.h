/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_CPP11_H
#define TURF_IMPL_MUTEX_CPP11_H

#include <turf/Core.h>
#include <mutex>

namespace turf {

class Mutex_CPP11 : protected std::recursive_mutex {
private:
    friend class LockGuard<Mutex_CPP11>;

public:
    Mutex_CPP11() : std::recursive_mutex() {
    }

    void lock() {
        std::recursive_mutex::lock();
    }

    bool tryLock() {
        return std::recursive_mutex::try_lock();
    }

    void unlock() {
        std::recursive_mutex::unlock();
    }
};

// Specialize LockGuard<Mutex_CPP11> so that ConditionVariable_CPP11 can use it:
template <>
class LockGuard<Mutex_CPP11> : public std::unique_lock<std::recursive_mutex> {
public:
    LockGuard(Mutex_CPP11& mutex) : std::unique_lock<std::recursive_mutex>(mutex) {
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_CPP11_H
