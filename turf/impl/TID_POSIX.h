/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TID_POSIX_H
#define TURF_IMPL_TID_POSIX_H

// Turf TIDs are integers (of some size) that are mainly used for logging and debugging.
// Ideally, Turf TIDs match the thread IDs shown in the debugger.

#include <turf/Core.h>
#include <turf/Util.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#ifdef TURF_KERNEL_FREEBSD
#include <pthread_np.h>
#endif

namespace turf {

class TID_POSIX {
public:
    // This only works when pthread_t is an integer type, as it is in the GNU C Library >= 2.3.3.
    // If that's not true for your Pthreads library, we'll need to extend Turf to fetch TIDs
    // from somewehere else in the environment.
#ifdef TURF_TARGET_MINGW
    typedef uptr TID;
#else
    typedef util::BestFit<pthread_t>::Unsigned TID;
#endif
    typedef util::BestFit<pid_t>::Unsigned PID;

    static TID getCurrentThreadID() {
        // FIXME: On Linux, would the kernel task ID be more useful for debugging?
        // If so, detect NPTL at compile time and create TID_NPTL.h which uses gettid() instead.
#ifdef TURF_KERNEL_FREEBSD
		return pthread_getthreadid_np();
#elif TURF_TARGET_MINGW
        return (TID) pthread_self().p;
#else
        return pthread_self();
#endif
    }

    static PID getCurrentProcessID() {
        return getpid();
    }
};

} // namespace turf

#endif // TURF_IMPL_TID_POSIX_H
