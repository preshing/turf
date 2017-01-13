/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_ASSERT_H
#define TURF_ASSERT_H

#include <turf/Core.h>

// clang-format off

#if TURF_WITH_ASSERTS
    // Use TURF_FORCE_CRASH() since TURF_DEBUG_BREAK() is silently ignored when stepping through Visual Studio:
    #define TURF_ASSERT(cond) do { \
        if (!(cond)) \
            TURF_FORCE_CRASH(); \
    } while (0)
#else
    #define TURF_ASSERT(cond) do {} while (0)
#endif

#endif // TURF_ASSERT_H
