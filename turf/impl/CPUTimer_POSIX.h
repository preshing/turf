/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CPUTIMER_POSIX_H
#define TURF_IMPL_CPUTIMER_POSIX_H

#include <stddef.h>
#include <time.h>

namespace turf {

struct CPUTimer_POSIX {
    typedef int64_t Duration;

    struct Point {
        uint64_t tick;
        Point(uint64_t tick = 0) : tick(tick) {
        }
        Point operator+(Duration d) const {
            return Point(tick + d);
        }
        Duration operator-(Point b) const {
            return tick - b.tick;
        }
        bool operator<(Point b) const {
            return (Duration)(tick - b.tick) < 0; // Handles wrap-around
        }
        bool operator>=(Point b) const {
            return (Duration)(tick - b.tick) >= 0; // Handles wrap-around
        }
    };

    static Point get() {
        struct timespec tick;
        clock_gettime(CLOCK_MONOTONIC, &tick);
        return Point((uint64_t) tick.tv_sec * 1000000000ull + tick.tv_nsec);
    }

    struct Converter {
        float toSeconds(Duration duration) const {
            return duration * 1e-9f;
        }
        Duration toDuration(float seconds) const {
            return (Duration)(seconds * 1e9f);
        }
    };
};

} // namespace turf

#endif // TURF_IMPL_CPUTIMER_POSIX_H
