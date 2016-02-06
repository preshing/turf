/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_THREAD_WIN32_H
#define TURF_IMPL_THREAD_WIN32_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {

#define TURF_THREAD_STARTCALL WINAPI

class Thread_Win32 {
private:
    HANDLE m_handle;

public:
    typedef DWORD ReturnType;
    typedef PTHREAD_START_ROUTINE StartRoutine;

    Thread_Win32() : m_handle(INVALID_HANDLE_VALUE) {
    }

    Thread_Win32(StartRoutine startRoutine, void* arg = NULL) {
        m_handle = CreateThread(NULL, 0, startRoutine, arg, 0, NULL);
    }

    ~Thread_Win32() {
        if (m_handle != INVALID_HANDLE_VALUE)
            CloseHandle(m_handle);
    }

    void join() {
        TURF_ASSERT(m_handle != INVALID_HANDLE_VALUE);
        WaitForSingleObject(m_handle, INFINITE);
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
    }

    void run(StartRoutine startRoutine, void* arg = NULL) {
        TURF_ASSERT(m_handle == INVALID_HANDLE_VALUE);
        m_handle = CreateThread(NULL, 0, startRoutine, arg, 0, NULL);
    }

    static void sleepMillis(ureg millis) {
        Sleep(millis);
    }
};

} // namespace turf

#endif // TURF_IMPL_THREAD_WIN32_H
