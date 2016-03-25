/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_CONDITIONVARIABLE_H
#define TURF_CONDITIONVARIABLE_H

#include <turf/Core.h>
#include <turf/Mutex.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_CONDITIONVARIABLE_PATH)
    #if TURF_PREFER_CPP11
        #define TURF_IMPL_CONDITIONVARIABLE_PATH "impl/ConditionVariable_CPP11.h"
        #define TURF_IMPL_CONDITIONVARIABLE_TYPE turf::ConditionVariable_CPP11
    // FIXME: Implement ConditionVariable_Boost
    #elif TURF_TARGET_WIN32
        #define TURF_IMPL_CONDITIONVARIABLE_PATH "impl/ConditionVariable_Win32.h"
        #define TURF_IMPL_CONDITIONVARIABLE_TYPE turf::ConditionVariable_Win32
    #elif TURF_TARGET_POSIX
        #define TURF_IMPL_CONDITIONVARIABLE_PATH "impl/ConditionVariable_POSIX.h"
        #define TURF_IMPL_CONDITIONVARIABLE_TYPE turf::ConditionVariable_POSIX
    #else
        #define TURF_IMPL_CONDITIONVARIABLE_PATH "*** Unable to select a default ConditionVariable implementation ***"
    #endif
#endif

// Include the implementation:
#include TURF_IMPL_CONDITIONVARIABLE_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_CONDITIONVARIABLE_TYPE ConditionVariable;
}

#endif // TURF_CONDITIONVARIABLE_H
