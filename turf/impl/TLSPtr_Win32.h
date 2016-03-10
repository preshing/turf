/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TLS_PTR_WIN32_H
#define TURF_IMPL_TLS_PTR_WIN32_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {

template<typename T>
class TLSPtr_Win32 {

private:

    DWORD m_tlsIndex;

    // NOT COPYABLE
    TLSPtr_Win32(const TLSPtr_Win32&);
    TLSPtr_Win32& operator=(const TLSPtr_Win32&);

public: // STRUCTORS

    TLSPtr_Win32() {
        m_tlsIndex = TlsAlloc();
        TURF_ASSERT(m_tlsIndex != TLS_OUT_OF_INDEXES);
    }

    ~TLSPtr_Win32() {
        LPVOID value = TlsGetValue(m_tlsIndex);
        TURF_ASSERT(GetLastError() == ERROR_SUCCESS);
        delete reinterpret_cast<T*>(value);
        TlsFree(m_tlsIndex);
    }

public: // ACCESSORS

    T* getData() const {
        LPVOID value = TlsGetValue(m_tlsIndex);
        TURF_ASSERT(GetLastError() == ERROR_SUCCESS);
        return reinterpret_cast<T*>(value);
    }

    T* operator->() const {
        return getData();
    }

    T& operator*() const {
        return *getData();
    }

public: // MUTATORS

    void setData(T* value) {
        LPVOID oldValue = TlsGetValue(m_tlsIndex);
        if (GetLastError() == ERROR_SUCCESS)
        {
            delete reinterpret_cast<T*>(oldValue);
        }
        BOOL result = TlsSetValue(m_tlsIndex, value);
        TURF_ASSERT(result != 0);
    }
};

} // namespace turf

#endif // TURF_IMPL_TLS_PTR_WIN32_H
