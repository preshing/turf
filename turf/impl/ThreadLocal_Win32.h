/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2017 Jeff Preshing and Bret Alfieri

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
class ThreadLocalScope_Win32;

template<typename T>
class ThreadLocal_Win32 {
private:
    TURF_STATIC_ASSERT(sizeof(T) <= TURF_PTR_SIZE);
    DWORD m_tlsIndex;

    ThreadLocal_Win32(const ThreadLocal_Win32&) {
    }

public:
    typedef ThreadLocalScope_Win32<T> Scope;

    ThreadLocal_Win32() {
        m_tlsIndex = TlsAlloc();
        TURF_ASSERT(m_tlsIndex != TLS_OUT_OF_INDEXES);
    }

    ~ThreadLocal_Win32() {
        BOOL rc = TlsFree(m_tlsIndex);
        TURF_ASSERT(rc != 0);
        TURF_UNUSED(rc);
    }

    T load() const {
        LPVOID value = TlsGetValue(m_tlsIndex);
        TURF_ASSERT(value != 0 || GetLastError() == ERROR_SUCCESS);
        return reinterpret_cast<T>(value);
    }

    operator T() const {
        return load();
    }

    // If <type_traits> is available (C++11), overload -> and * for pointer types.
    // Note: This could probably be done in C++98 by using SFINAE directly.
#if TURF_HAS_TYPE_TRAITS
    typename std::enable_if<std::is_pointer<T>::value, T>::type operator->() const {
        return load();
    }

    typename std::enable_if<std::is_pointer<T>::value, typename std::remove_pointer<T>::type>::type operator*() const {
        return *load();
    }
#endif // TURF_HAS_TYPE_TRAITS

    void operator=(T value) {
        BOOL rc = TlsSetValue(m_tlsIndex, value);
        TURF_ASSERT(rc != 0);
        TURF_UNUSED(rc);
    }

#if TURF_HAS_MOVE_SEMANTICS
    // In C++11, you can write auto scope = myTLvar.setInScope(value);
    ThreadLocalScope_Win32<T> setInScope(T value);
#endif
};

template<typename T>
class ThreadLocalScope_Win32 {
private:
    ThreadLocal_Win32<T>* m_var;
    T m_oldValue;

    ThreadLocalScope_Win32(const ThreadLocalScope_Win32&) {
    }

public:
    ThreadLocalScope_Win32(ThreadLocal_Win32<T>& ptr, T value) : m_var(&ptr) {
        m_oldValue = *m_var;
        *m_var = value;
    }

#if TURF_HAS_MOVE_SEMANTICS
    ThreadLocalScope_Win32(ThreadLocalScope_Win32&& other) {
        m_var = other.m_var;
        m_oldValue = other.m_oldValue;
        other.m_var = nullptr;
    }
#endif

    ~ThreadLocalScope_Win32() {
        if (m_var) {
            *m_var = m_oldValue;
        }
    }
};

#if TURF_HAS_MOVE_SEMANTICS
template<typename T>
ThreadLocalScope_Win32<T> ThreadLocal_Win32<T>::setInScope(T value) {
    return ThreadLocalScope_Win32<T>(*this, value);
}
#endif

} // namespace turf

#endif // TURF_IMPL_THREADLOCAL_WIN32_H
