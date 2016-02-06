/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MEMPAGE_POSIX_H
#define TURF_IMPL_MEMPAGE_POSIX_H

#include <turf/Core.h>
#include <unistd.h>
#include <sys/mman.h>

namespace turf {

class MemPage_POSIX {
public:
    static ureg getPageSize(ureg& allocAlignment) {
        long result = sysconf(_SC_PAGE_SIZE);
        allocAlignment = result;
        return result;
    }

    static bool alloc(void*& result, ureg size, bool topDownHint = false) {
        result = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        TURF_ASSERT(result != MAP_FAILED);
        return true;
    }

    static bool free(void* ptr, ureg size) {
        munmap(ptr, size);
        return true;
    }
};

} // namespace turf

#endif // TURF_IMPL_MEMPAGE_POSIX_H
