/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MANUALRESETEVENT_WIN32_H
#define TURF_IMPL_MANUALRESETEVENT_WIN32_H

#include <turf/Core.h>

namespace turf {

class ManualResetEvent_Win32 {
private:
    HANDLE m_evt;

public:
    ManualResetEvent_Win32(bool initialState = false) {
        m_evt = CreateEvent(NULL, TRUE, initialState ? TRUE : FALSE, NULL);
    }

    ~ManualResetEvent_Win32() {
        CloseHandle(m_evt);
    }

    void signal() {
        SetEvent(m_evt);
    }

    void reset() {
        ResetEvent(m_evt);
    }

    void wait() {
        WaitForSingleObject(m_evt, INFINITE);
    }
};

} // namespace turf

#endif // TURF_IMPL_MANUALRESETEVENT_WIN32_H
