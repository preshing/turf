/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_TRACE_H
#define TURF_TRACE_H

#include <turf/Core.h>

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_TRACE_PATH)
    #define TURF_IMPL_TRACE_PATH "impl/Trace_Null.h"
    #define TURF_IMPL_TRACE_TYPE turf::Trace_Null
#endif

// Include the implementation:
#include TURF_IMPL_TRACE_PATH

// Alias it:
namespace turf {
typedef TURF_IMPL_TRACE_TYPE Trace;
}

#endif // TURF_TRACE_H
