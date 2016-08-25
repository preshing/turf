/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CPUTIMER_GCC_H
#define TURF_IMPL_CPUTIMER_GCC_H

namespace turf {

struct CPUTimer_GCC {
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

    // clang-format off
	static Point get() {
		#if TURF_CPU_X64 || TURF_CPU_X86
			#if TURF_USE_RDTSCP
			    uint64_t tickl, tickh;
			    asm volatile("rdtscp" : "=a"(tickl), "=d"(tickh) :: "memory", "%rcx");
			    return Point(((uint64_t) tickh << 32) | tickl);
			#else
			    //turf_instructionFence();  // FIXME: Insert a CPUID instruction here for more stable measurements
			    uint32_t tickl, tickh;
			    asm volatile("rdtsc" : "=a"(tickl), "=d"(tickh) :: "memory");
			    return Point(((uint64_t) tickh << 32) | tickl);
			#endif
		#elif TURF_CPU_ARM
			#error "Not implemented yet!"
		#else
			#error "Unsupported platform!"	    
		#endif
	}
    // clang-format on

    struct Converter {
        float ticksPerSecond;
        float secondsPerTick;
        Converter();
        float toSeconds(Duration duration) const {
            return duration * secondsPerTick;
        }
        Duration toDuration(float seconds) const {
            return (Duration)(seconds * ticksPerSecond);
        }
    };
};

} // namespace turf

#endif // TURF_IMPL_CPUTIMER_GCC_H
