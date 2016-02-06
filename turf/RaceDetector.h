/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_RACEDETECTOR_H
#define TURF_RACEDETECTOR_H

#include <turf/Core.h>

namespace turf {

#if TURF_WITH_ASSERTS

struct RaceDetector {
    turf::Atomic<bool> entered;

    RaceDetector() : entered(false) {
    }
};

class RaceDetectGuard {
private:
    RaceDetector& m_guard;

public:
    RaceDetectGuard(RaceDetector& guard) : m_guard(guard) {
        if (m_guard.entered.exchange(true, turf::Acquire) == true)
            TURF_DEBUG_BREAK();
    }
    ~RaceDetectGuard() {
        m_guard.entered.store(false, turf::Release);
    }
};

#define TURF_DEFINE_RACE_DETECTOR(name) turf::RaceDetector name;
#define TURF_RACE_DETECT_GUARD(name) turf::RaceDetectGuard TURF_UNIQUE_VARIABLE(raceDetectGuard)(name)

#else

// clang-format off
#define TURF_DEFINE_RACE_DETECTOR(name)
#define TURF_RACE_DETECT_GUARD(name) do {} while (0)
// clang-format on

#endif

} // namespace turf

#endif // TURF_RACEDETECTOR_H
