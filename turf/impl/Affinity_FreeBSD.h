/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_AFFINITY_FREEBSD_H
#define TURF_IMPL_AFFINITY_FREEBSD_H

#include <turf/Core.h>
#include <pthread.h>
#include <sched.h>
#include <vector>

namespace turf {

class Affinity_FreeBSD {
private:
    struct CoreInfo {
        std::vector<u32> hwThreadIndexToLogicalProcessor;
    };
    bool m_isAccurate;
    std::vector<CoreInfo> m_coreIndexToInfo;
    u32 m_numHWThreads;

public:
    Affinity_FreeBSD();

    bool isAccurate() const {
        return m_isAccurate;
    }

    u32 getNumPhysicalCores() const {
        return m_coreIndexToInfo.size();
    }

    u32 getNumHWThreads() const {
        return m_numHWThreads;
    }

    u32 getNumHWThreadsForCore(ureg core) const {
        return m_coreIndexToInfo[core].hwThreadIndexToLogicalProcessor.size();
    }

    bool setAffinity(ureg core, ureg hwThread);
};

} // namespace turf

#endif // TURF_IMPL_AFFINITY_FREEBSD_H
