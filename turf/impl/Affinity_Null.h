/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_AFFINITY_NULL_H
#define TURF_IMPL_AFFINITY_NULL_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {

class Affinity_Null {
public:
    bool isAccurate() const {
        return false;
    }

    u32 getNumPhysicalCores() const {
        return 1;
    }

    u32 getNumHWThreads() const {
        return 1;
    }

    u32 getNumHWThreadsForCore(ureg core) const {
        TURF_UNUSED(core);
        return 1;
    }

    bool setAffinity(ureg core, ureg hwThread) {
        TURF_UNUSED(core);
        TURF_UNUSED(hwThread);
        return true;
    }
};

} // namespace turf

#endif // TURF_IMPL_AFFINITY_NULL_H
