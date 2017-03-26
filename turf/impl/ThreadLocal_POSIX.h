/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2017 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------------*/

#ifndef TURF_IMPL_THREADLOCAL_POSIX_H
#define TURF_IMPL_THREADLOCAL_POSIX_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <pthread.h>

namespace turf {

template<typename T>
class ThreadLocalScope_POSIX;

template<typename T>
class ThreadLocal_POSIX {
private:
    TURF_STATIC_ASSERT(sizeof(T) <= TURF_PTR_SIZE);
    pthread_key_t m_tlsKey;

    ThreadLocal_POSIX(const ThreadLocal_POSIX&) {
    }

public:
    typedef ThreadLocalScope_POSIX<T> Scope;

    ThreadLocal_POSIX() {
        int rc = pthread_key_create(&m_tlsKey, NULL);
        TURF_ASSERT(rc == 0);
        TURF_UNUSED(rc);
    }

    ~ThreadLocal_POSIX() {
        int rc = pthread_key_delete(m_tlsKey);
        TURF_ASSERT(rc == 0);
        TURF_UNUSED(rc);
    }

    T load() const {
        void* value = pthread_getspecific(m_tlsKey);
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
        int rc = pthread_setspecific(m_tlsKey, value);
        TURF_ASSERT(rc == 0);
        TURF_UNUSED(rc);
    }

    void operator=(const ThreadLocal_POSIX<T>& other) {
        *this = other.load();
    }

#if TURF_HAS_MOVE_SEMANTICS
    // In C++11, you can write auto scope = myTLvar.setInScope(value);
    ThreadLocalScope_POSIX<T> setInScope(T value);
#endif
};

template<typename T>
class ThreadLocalScope_POSIX {
private:
    ThreadLocal_POSIX<T>* m_var;
    T m_oldValue;

    ThreadLocalScope_POSIX(const ThreadLocalScope_POSIX&) {
    }

public:
    ThreadLocalScope_POSIX(ThreadLocal_POSIX<T>& ptr, T value) : m_var(&ptr) {
        m_oldValue = *m_var;
        *m_var = value;
    }

#if TURF_HAS_MOVE_SEMANTICS
    ThreadLocalScope_POSIX(ThreadLocalScope_POSIX&& other) {
        m_var = other.m_var;
        m_oldValue = other.m_oldValue;
        other.m_var = nullptr;
    }
#endif

    ~ThreadLocalScope_POSIX() {
        if (m_var) {
            *m_var = m_oldValue;
        }
    }
};

#if TURF_HAS_MOVE_SEMANTICS
template<typename T>
ThreadLocalScope_POSIX<T> ThreadLocal_POSIX<T>::setInScope(T value) {
    return ThreadLocalScope_POSIX<T>(*this, value);
}
#endif

} // namespace turf

#endif // TURF_IMPL_THREADLOCAL_POSIX_H
