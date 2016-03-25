/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TID_MACH_H
#define TURF_IMPL_TID_MACH_H

#include <turf/Core.h>
#include <turf/Util.h>
#include <pthread.h>
#include <mach/mach_types.h>
#include <unistd.h>

namespace turf {

class TID_Mach {
public:
    typedef util::BestFit<thread_port_t>::Unsigned TID;
    typedef util::BestFit<pid_t>::Unsigned PID;

    static TID getCurrentThreadID() {
        return pthread_mach_thread_np(pthread_self());
    }

    static PID getCurrentProcessID() {
        return getpid();
    }
};

} // namespace turf

#endif // TURF_IMPL_TID_MACH_H
