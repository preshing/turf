/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_AFFINITY_WIN32_H
#define TURF_IMPL_AFFINITY_WIN32_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <turf/Util.h>

namespace turf {

class Affinity_Win32 {
private:
    typedef ULONG_PTR AffinityMask;
    static const ureg MaxHWThreads = sizeof(AffinityMask) * 8;

    bool m_isAccurate;
    ureg m_numPhysicalCores;
    ureg m_numHWThreads;
    AffinityMask m_physicalCoreMasks[MaxHWThreads];

public:
    Affinity_Win32();

    bool isAccurate() const {
        return m_isAccurate;
    }

    u32 getNumPhysicalCores() const {
        return static_cast<u32>(m_numPhysicalCores);
    }

    u32 getNumHWThreads() const {
        return static_cast<u32>(m_numHWThreads);
    }

    u32 getNumHWThreadsForCore(ureg core) const {
        TURF_ASSERT(core < m_numPhysicalCores);
        return static_cast<u32>(util::countSetBits(m_physicalCoreMasks[core]));
    }

    bool setAffinity(ureg core, ureg hwThread);
};

} // namespace turf

#endif // TURF_IMPL_AFFINITY_WIN32_H
