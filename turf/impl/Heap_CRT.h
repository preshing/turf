/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_HEAP_CRT_H
#define TURF_IMPL_HEAP_CRT_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <turf/Util.h>
#if TURF_TARGET_WIN32
#include <malloc.h>
#else
#include <stdlib.h>
#endif // TURF_TARGET_WIN32

namespace turf {

class Heap_CRT {
public:
    class Operator {
    public:
        void* alloc(ureg size) {
            return ::malloc((size_t) size);
        }

        void* realloc(void* ptr, ureg newSize) {
            return ::realloc(ptr, newSize);
        }

        void free(void* ptr) {
            ::free(ptr);
        }

        void* allocAligned(ureg size, ureg alignment) {
            TURF_ASSERT(util::isPowerOf2(alignment));
#if TURF_TARGET_WIN32
            return ::_aligned_malloc((size_t) size, (size_t) alignment);
#else
            void* ptr;
            int rc = posix_memalign(&ptr, util::max<size_t>(alignment, TURF_PTR_SIZE), (size_t) size);
            TURF_ASSERT(rc == 0);
            TURF_UNUSED(rc);
            return ptr;
#endif // TURF_TARGET_WIN32
        }

        void freeAligned(void* ptr) {
#if TURF_TARGET_WIN32
            ::_aligned_free(ptr);
#else
            ::free(ptr);
#endif // TURF_TARGET_WIN32
        }
    };

    Operator operate(const char*) {
        return Operator();
    }
};

} // namespace turf

#endif // TURF_IMPL_HEAP_CRT_H
