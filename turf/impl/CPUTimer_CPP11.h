/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_CPUTIMER_CPP11_H
#define TURF_IMPL_CPUTIMER_CPP11_H

#include <turf/Core.h>
#include <chrono>

namespace turf {

struct CPUTimer_CPP11 {
    struct Duration : std::chrono::high_resolution_clock::duration {
        typedef std::chrono::high_resolution_clock::duration Base;
        Duration(Base b) : Base(b) {
        }
        Duration(Base::rep rep) : Base(rep) {
        }
        // Add the ability to cast to u64 (for random seeds):
        operator u64() const {
            return u64(count());
        }
    };

    struct Point : std::chrono::high_resolution_clock::time_point {
        typedef std::chrono::high_resolution_clock::time_point Base;
        Point(Base b) : Base(b) {
        }
        // Add the ability construct from an arithmetic type (for random seeds):
        Point(Duration::rep rep) : Base(Duration(rep)) {
        }
        Duration operator-(const Point& b) const {
            return Duration(Base(*this) - Base(b));
        }
    };

    static Point get() {
        return Point(std::chrono::high_resolution_clock::now());
    }

    struct Converter {
        Converter() {
        }
        static float toSeconds(Duration duration) {
            return std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
        }
    };
};

} // namespace turf

#endif // TURF_IMPL_CPUTIMER_CPP11_H
