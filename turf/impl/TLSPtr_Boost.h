/*------------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TLS_PTR_BOOST_H
#define TURF_IMPL_TLS_PTR_BOOST_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <boost/thread/tss.hpp>

namespace turf {

template<typename T>
class TLSPtr_Boost {

private:

    boost::thread_specific_ptr<T> m_tssPtr;

    // NOT COPYABLE
    TLSPtr_Boost(const TLSPtr_Boost&);
    TLSPtr_Boost& operator=(const TLSPtr_Boost&);

public: // STRUCTORS

    TLSPtr_Boost() : m_tssPtr(0) {
    }

public: // ACCESSORS

    T* getData() const {
        return m_tssPtr.get();
    }

    T* operator->() const {
        return m_tssPtr.get();
    }

    T& operator*() const {
        T* data = getData();
        TURF_ASSERT(data);
        return *data;
    }

public: // MUTATORS

    void setData(T* value) {
        m_tssPtr.reset(value);
    }
};

} // namespace turf

#endif // TURF_IMPL_TLS_PTR_BOOST_H
