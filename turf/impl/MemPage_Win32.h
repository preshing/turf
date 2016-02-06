/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_MEMPAGE_WIN32_H
#define TURF_IMPL_MEMPAGE_WIN32_H

#include <turf/Core.h>

namespace turf {

class MemPage_Win32 {
public:
    static ureg getPageSize(ureg& allocAlignment) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        allocAlignment = sysInfo.dwAllocationGranularity;
        return sysInfo.dwPageSize;
    }

    static bool alloc(void*& result, ureg size, bool topDownHint = false) {
        DWORD type = MEM_RESERVE | MEM_COMMIT;
        if (topDownHint)
            type |= MEM_TOP_DOWN;
        result = VirtualAlloc(0, (SIZE_T) size, type, PAGE_READWRITE);
        return (result != NULL);
    }

    static bool free(void* ptr, ureg size) {
        MEMORY_BASIC_INFORMATION minfo;
        while (sreg(size) > 0) {
            if (VirtualQuery((LPCVOID) ptr, &minfo, sizeof(minfo)) == 0)
                return false;
            if (minfo.BaseAddress != ptr || minfo.AllocationBase != ptr || minfo.State != MEM_COMMIT || minfo.RegionSize > size)
                return false;
            if (VirtualFree(ptr, 0, MEM_RELEASE) == 0)
                return false;
            ptr = TURF_PTR_OFFSET(ptr, minfo.RegionSize);
            size -= minfo.RegionSize;
        }
        return true;
    }
};

} // namespace turf

#endif // TURF_IMPL_MEMPAGE_WIN32_H
