/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------------*/

#ifndef TURF_IMPL_THREADLOCAL_WIN32_H
#define TURF_IMPL_THREADLOCAL_WIN32_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {

template<typename T>
class ThreadLocal_Win32 {
private:
    TURF_STATIC_ASSERT(sizeof(T) <= TURF_PTR_SIZE);
    DWORD m_tlsIndex;

public:
    ThreadLocal_Win32() {
        m_tlsIndex = TlsAlloc();
        TURF_ASSERT(m_tlsIndex != TLS_OUT_OF_INDEXES);
    }

    ~ThreadLocal_Win32() {
        BOOL rc = TlsFree(m_tlsIndex);
        TURF_ASSERT(rc != 0);
        TURF_UNUSED(rc);
    }

    T get() const {
        LPVOID value = TlsGetValue(m_tlsIndex);
        TURF_ASSERT(value != 0 || GetLastError() == ERROR_SUCCESS);
        return reinterpret_cast<T>(value);
    }

    operator T() const {
        return get();
    }

    void set(T value) {
        BOOL rc = TlsSetValue(m_tlsIndex, value);
        TURF_ASSERT(rc != 0);
        TURF_UNUSED(rc);
    }

    void operator=(T value) {
        set(value);
    }
};

} // namespace turf

#endif // TURF_IMPL_THREADLOCAL_WIN32_H
