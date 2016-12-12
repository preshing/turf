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
#include <malloc.h>

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
            return ::_aligned_malloc((size_t) size, (size_t) alignment);
        }

        void freeAligned(void* ptr) {
            ::_aligned_free(ptr);
        }
    };

    Operator operate(const char*) {
        return Operator();
    }
};

} // namespace turf

#endif // TURF_IMPL_HEAP_CRT_H
