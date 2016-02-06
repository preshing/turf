/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_SEMAPHORE_WIN32_H
#define TURF_IMPL_SEMAPHORE_WIN32_H

#include <turf/Core.h>

namespace turf {

class Semaphore_Win32 {
private:
    HANDLE m_sem;

public:
    Semaphore_Win32() {
        m_sem = CreateSemaphore(NULL, 0, INT32_MAX, NULL);
    }

    ~Semaphore_Win32() {
        CloseHandle(m_sem);
    }

    void wait() {
        WaitForSingleObject(m_sem, INFINITE);
    }

    void signal(ureg count = 1) {
        ReleaseSemaphore(m_sem, count, NULL);
    }
};

} // namespace turf

#endif // TURF_IMPL_SEMAPHORE_WIN32_H
