/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_RWLOCK_CPP14_H
#define TURF_IMPL_RWLOCK_CPP14_H

#include <turf/Core.h>
#include <turf/c/rwLock.h>

namespace turf {

class RWLock_CPP14 : protected std::shared_mutex {
public:
    RWLock_CPP14() : std::shared_mutex() {
    }

    void lockExclusive() {
        std::shared_mutex::lock();
    }

    void unlockExclusive() {
        std::shared_mutex::unlock();
    }

    void lockShared() {
        std::shared_mutex::lockShared();
    }

    void unlockShared() {
        std::shared_mutex::unlockShared();
    }
};

} // namespace turf

#endif // TURF_IMPL_RWLOCK_CPP14_H
