/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MUTEX_BOOST_H
#define TURF_IMPL_MUTEX_BOOST_H

#include <turf/Core.h>
#include <boost/thread/mutex.hpp>

namespace turf {

class Mutex_Boost : protected boost::mutex {
private:
    friend class LockGuard<Mutex_Boost>;

public:
    Mutex_Boost() : boost::mutex() {
    }

    void lock() {
        boost::mutex::lock();
    }

    bool tryLock() {
        return boost::mutex::try_lock();
    }

    void unlock() {
        boost::mutex::unlock();
    }
};

// Specialize LockGuard<Mutex_Boost> so that ConditionVariable_Boost can use it:
template <>
class LockGuard<Mutex_Boost> : public boost::mutex::scoped_lock {
public:
    LockGuard(Mutex_Boost& mutex) : boost::mutex::scoped_lock(mutex) {
    }
};

} // namespace turf

#endif // TURF_IMPL_MUTEX_BOOST_H
