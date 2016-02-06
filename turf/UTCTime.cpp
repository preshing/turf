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
#include <turf/UTCTime.h>

#if TURF_TARGET_WIN32

namespace turf {
uint64_t getCurrentUTCTime() {
    FILETIME fileTime;
    ULARGE_INTEGER largeInteger;

    GetSystemTimeAsFileTime(&fileTime);
    largeInteger.LowPart = fileTime.dwLowDateTime;
    largeInteger.HighPart = fileTime.dwHighDateTime;
    return largeInteger.QuadPart / 10;
}
} // namespace turf

#elif TURF_TARGET_POSIX

#if TURF_USE_POSIX_2008_CLOCK
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace turf {
uint64_t getCurrentUTCTime() {
#if TURF_USE_POSIX_2008_CLOCK
    struct timespec tick;
    clock_gettime(CLOCK_REALTIME, &tick);
    return (uint64_t) tick.tv_sec * 1000000ull + tick.tv_nsec / 1000 + 11644473600000000ull;
#else
    struct timeval tick;
    gettimeofday(&tick, NULL);
    return (uint64_t) tick.tv_sec * 1000000ull + tick.tv_usec + 11644473600000000ull;
#endif
}
} // namespace turf

#endif
