/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TLS_PTR_POSIX_H
#define TURF_IMPL_TLS_PTR_POSIX_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <pthread.h>

namespace turf {

template<typename T>
class TLSPtr_POSIX {

private:

    pthread_key_t m_tlsKey;

    // NOT COPYABLE
    TLSPtr_POSIX(const TLSPtr_POSIX&);
    TLSPtr_POSIX& operator=(const TLSPtr_POSIX&);

public: // STRUCTORS

    TLSPtr_POSIX() : m_tlsKey(0) {
        int result = pthread_key_create(&m_tlsKey, NULL);
        TURF_ASSERT(result == 0);
    }

    ~TLSPtr_POSIX() {
        void* value = pthread_getspecific(m_tlsKey);
        delete reinterpret_cast<T*>(value);
        int result = pthread_key_delete(m_tlsKey);
        TURF_ASSERT(result == 0);
    }

public: // ACCESSORS

    T* getData() const {
        void* value = pthread_getspecific(m_tlsKey);
        return reinterpret_cast<T*>(value);
    }

    T* operator->() const {
        return getData();
    }

    T& operator*() const {
        T* data = getData();
        TURF_ASSERT(data);
        return *data;
    }

public: // MUTATORS

    void setData(T* value) {
        void* oldValue = pthread_getspecific(m_tlsKey);
        delete reinterpret_cast<T*>(oldValue);
        int result = pthread_setspecific(m_tlsKey, value);
        TURF_ASSERT(result == 0);
    }
};

} // namespace turf

#endif // TURF_IMPL_TLS_PTR_POSIX_H
