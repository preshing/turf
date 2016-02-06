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
#include <turf/extra/TimeWaster.h>
#include <turf/extra/Random.h>

namespace turf {
namespace extra {

u32 TimeWaster::s_randomValues[ArraySize];

void TimeWaster::initialize() {
    Random random;
    for (u32 i = 0; i < ArraySize; i++)
        s_randomValues[i] = random.next32();
}

TimeWaster::TimeWaster() : m_pos(0) {
    Random random;
    m_pos = random.next32() % ArraySize;
    m_step = random.next32() % (ArraySize - 1) + 1;
}

void TimeWaster::wasteRandomCycles() {
    uint32_t i;
    do {
        i = s_randomValues[m_pos];
        m_pos += m_step;
        if (m_pos >= ArraySize)
            m_pos -= ArraySize;
    } while (i & 7);
}

} // namespace extra
} // namespace turf
