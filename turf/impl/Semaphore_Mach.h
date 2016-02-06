/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_SEMAPHORE_MACH_H
#define TURF_IMPL_SEMAPHORE_MACH_H

#include <turf/Core.h>
#include <mach/mach.h>

namespace turf {

class Semaphore_Mach {
private:
    semaphore_t m_semaphore;

public:
    Semaphore_Mach() {
        semaphore_create(mach_task_self(), &m_semaphore, SYNC_POLICY_FIFO, 0);
    }

    ~Semaphore_Mach() {
        semaphore_destroy(mach_task_self(), m_semaphore);
    }

    void wait() {
        semaphore_wait(m_semaphore);
    }

    void signal(ureg count = 1) {
        while (count-- > 0)
            semaphore_signal(m_semaphore);
    }
};

} // namespace turf

#endif // TURF_IMPL_SEMAPHORE_MACH_H
