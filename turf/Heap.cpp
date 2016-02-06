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
#include <turf/Heap.h>

TURF_IMPL_HEAP_TYPE TurfHeap;

//---------------------------------------------------------------------------
// Override operators new/delete
// C++ allows us to replace global operators new/delete with our own thanks to
// weak linking.
//---------------------------------------------------------------------------
#if TURF_REPLACE_OPERATOR_NEW
void* operator new(std::size_t size) {
    return TURF_HEAP.alloc(size);
}

void* operator new[](std::size_t size) {
    return TURF_HEAP.alloc(size);
}

void operator delete(void* ptr) TURF_NOEXCEPT {
    TURF_HEAP.free(ptr);
}

void operator delete[](void* ptr) TURF_NOEXCEPT {
    TURF_HEAP.free(ptr);
}

#if TURF_WITH_EXCEPTIONS
void* operator new(std::size_t size, std::nothrow_t const&) TURF_NOEXCEPT {
    return TURF_HEAP.alloc(size);
}

void* operator new[](std::size_t size, std::nothrow_t const&) TURF_NOEXCEPT {
    return TURF_HEAP.alloc(size);
}

void operator delete(void* ptr, std::nothrow_t const&) TURF_NOEXCEPT {
    TURF_HEAP.free(ptr);
}

void operator delete[](void* ptr, std::nothrow_t const&) TURF_NOEXCEPT {
    TURF_HEAP.free(ptr);
}
#endif // TURF_WITH_EXCEPTIONS
#endif // TURF_REPLACE_OPERATOR_NEW
