/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CPUTIMER_MACH_H
#define TURF_IMPL_CPUTIMER_MACH_H

#include <mach/mach_time.h>

namespace turf {

struct CPUTimer_Mach {
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
        return Point(mach_absolute_time());
    }

    struct Converter {
        float ticksPerSecond;
        float secondsPerTick;
        Converter() {
            mach_timebase_info_data_t info;
            mach_timebase_info(&info);
            ticksPerSecond = 1e9f * info.denom / info.numer;
            secondsPerTick = 1.0f / ticksPerSecond;
        }
        float toSeconds(Duration duration) const {
            return duration * secondsPerTick;
        }
        Duration toDuration(float seconds) const {
            return (Duration)(seconds * ticksPerSecond);
        }
    };
};

} // namespace turf

#endif // TURF_IMPL_CPUTIMER_MACH_H
