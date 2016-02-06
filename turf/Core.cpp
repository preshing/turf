/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <turf/Core.h>

namespace turf {

TURF_STATIC_ASSERT(sizeof(s8) == 1);
TURF_STATIC_ASSERT(sizeof(s16) == 2);
TURF_STATIC_ASSERT(sizeof(s32) == 4);
TURF_STATIC_ASSERT(sizeof(s64) == 8);
TURF_STATIC_ASSERT(sizeof(sptr) == sizeof(void*));

} // namespace turf
