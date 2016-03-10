/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing and Bret Alfieri

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_TLS_PTR_H
#define TURF_TLS_PTR_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_TLS_PTR_PATH)
    #if TURF_PREFER_BOOST
        #define TURF_IMPL_TLS_PTR_PATH "impl/TLSPtr_Boost.h"
        #define TURF_IMPL_TLS_PTR_TYPE turf::TLSPtr_Boost
    #elif TURF_TARGET_WIN32
        #define TURF_IMPL_TLS_PTR_PATH "impl/TLSPtr_Win32.h"
        #define TURF_IMPL_TLS_PTR_TYPE turf::TLSPtr_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_TLS_PTR_PATH "impl/TLSPtr_POSIX.h"
        #define TURF_IMPL_TLS_PTR_TYPE turf::TLSPtr_POSIX
    #else
        #define TURF_IMPL_TLS_PTR_PATH "*** Unable to select a default TLSPtr implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_TLS_PTR_PATH

// Alias it:
namespace turf {

template<typename T>
class TLSPtr : public TURF_IMPL_TLS_PTR_TYPE<T>
{};

}

#endif // TURF_TLS_PTR_H
