/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TID_WIN32_H
#define TURF_IMPL_TID_WIN32_H

#include <turf/Core.h>
#include <turf/Util.h>

namespace turf {

class TID_Win32 {
public:
    typedef util::BestFit<DWORD>::Unsigned TID;
    typedef util::BestFit<DWORD>::Unsigned PID;

    // clang-format off
    static TID getCurrentThreadID() {
#if TURF_CPU_X64                                 // Windows x64
        return ((DWORD*) __readgsqword(48))[18]; // Read directly from the TIB
#elif TURF_CPU_X86 // Windows x86
        return ((DWORD*) __readfsdword(24))[9]; // Read directly from the TIB
#else
        return GetCurrentThreadID();
#endif
    }

    static PID getCurrentProcessID() {
#if TURF_CPU_X64                                 // Windows x64
        return ((DWORD*) __readgsqword(48))[16]; // Read directly from the TIB
#elif TURF_CPU_X86         // Windows x86
        return ((DWORD*) __readfsdword(24))[8]; // Read directly from the TIB
#elif TURF_TARGET_XBOX_360 // Xbox 360
        return 0;
#else
        return GetCurrentProcessID();
#endif
    }
    // clang-format on
};

} // namespace turf

#endif // TURF_IMPL_TID_WIN32_H
