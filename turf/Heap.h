/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_HEAP_H
#define TURF_HEAP_H

#include <turf/Core.h>

// clang-format off

// Choose implementation based on turf_config.h:
#if TURF_USE_DLMALLOC
    #define TURF_IMPL_HEAP_PATH "impl/Heap_DL.h"
    #define TURF_IMPL_HEAP_TYPE turf::Heap_DL
#else
    #define TURF_IMPL_HEAP_PATH "impl/Heap_CRT.h"
    #define TURF_IMPL_HEAP_TYPE turf::Heap_CRT
#endif

// Include the implementation:
#include TURF_IMPL_HEAP_PATH

// Alias it:
extern TURF_IMPL_HEAP_TYPE TurfHeap;

#define TURF_HEAP_DIRECT(heap) (heap).operate(__FILE__ "(" TURF_STRINGIFY(__LINE__) ")")
#define TURF_HEAP TURF_HEAP_DIRECT(TurfHeap)

#endif // TURF_HEAP_H
