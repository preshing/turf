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
#include <turf/extra/Random.h>
#include <turf/UTCTime.h>
#include <turf/CPUTimer.h>
#include <turf/TID.h>
#include <turf/Util.h>

namespace turf {
namespace extra {

Random::Random() {
    // Seed using misc. information from the environment
    u64 t = getCurrentUTCTime();
    t = util::avalanche(t);
    s[0] = util::avalanche(t | 1);

    t = u64(CPUTimer::get() - CPUTimer::Point(0));
    t = util::avalanche(t) + (util::avalanche(TID::getCurrentThreadID()) << 1);
    s[1] = util::avalanche(t | 1);

    for (ureg i = 0; i < 10; i++)
        next64();
}

u64 Random::next64() {
    u64 s1 = s[0];
    const u64 s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;                                           // a
    return (s[1] = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0; // b, c
}

} // namespace extra
} // namespace turf
