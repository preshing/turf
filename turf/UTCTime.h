/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_UTCTIME_H
#define TURF_UTCTIME_H

#include <turf/Core.h>

namespace turf {

// Number of microseconds since January 1, 1601 in Coordinated Universal Time.
uint64_t getCurrentUTCTime();

} // namespace turf

#endif // TURF_UTCTIME_H
