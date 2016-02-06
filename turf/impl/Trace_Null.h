/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TRACE_NULL_H
#define TURF_IMPL_TRACE_NULL_H

#include <turf/Core.h>

namespace turf {

struct Trace_Null {
    void dumpStats() {
    }

    static Trace_Null Instance;
};

} // namespace turf

// clang-format off
#define TURF_TRACE_DECLARE(group, count)
#define TURF_TRACE_DEFINE_BEGIN(group, count)
#define TURF_TRACE_DEFINE(desc)
#define TURF_TRACE_DEFINE_END(group, count)
#define TURF_TRACE(group, id, str, param1, param2) do {} while (0)
// clang-format on

#endif // TURF_IMPL_TRACE_NULL_H
