/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_TIMEWASTER_H
#define TURF_EXTRA_TIMEWASTER_H

#include <turf/Core.h>

namespace turf {
namespace extra {

//-------------------------------------
//  TimeWaster
//-------------------------------------
class TimeWaster {
private:
    static const u32 ArraySize = 65521;
    static u32 s_randomValues[ArraySize];

    u32 m_pos;
    u32 m_step;

public:
    static void initialize();

    TimeWaster();
    void wasteRandomCycles();
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_TIMEWASTER_H
