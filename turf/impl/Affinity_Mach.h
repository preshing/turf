/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_AFFINITY_MACH_H
#define TURF_IMPL_AFFINITY_MACH_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <sys/sysctl.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>

namespace turf {

class Affinity_Mach {
private:
    bool m_isAccurate;
    u32 m_numHWThreads;
    u32 m_numPhysicalCores;
    u32 m_hwThreadsPerCore;

public:
    Affinity_Mach() : m_isAccurate(false), m_numHWThreads(1), m_numPhysicalCores(1), m_hwThreadsPerCore(1) {
        int count;
        // Get # of HW threads
        size_t countLen = sizeof(count);
        if (sysctlbyname("hw.logicalcpu", &count, &countLen, NULL, 0) == 0) {
            if (count > 0) {
                m_numHWThreads = (u32) count;
                // Get # of physical cores
                size_t countLen = sizeof(count);
                if (sysctlbyname("hw.physicalcpu", &count, &countLen, NULL, 0) == 0) {
                    if (count > 0) {
                        m_numPhysicalCores = count;
                        m_hwThreadsPerCore = u32(m_numHWThreads / count);
                        if (m_hwThreadsPerCore < 1)
                            m_hwThreadsPerCore = 1;
                        else
                            m_isAccurate = true;
                    }
                }
            }
        }
    }

    bool isAccurate() const {
        return m_isAccurate;
    }

    u32 getNumPhysicalCores() const {
        return m_numPhysicalCores;
    }

    u32 getNumHWThreads() const {
        return m_numHWThreads;
    }

    u32 getNumHWThreadsForCore(ureg core) const {
        TURF_ASSERT(core < m_numPhysicalCores);
        return m_hwThreadsPerCore;
    }

    bool setAffinity(ureg core, ureg hwThread) {
        TURF_ASSERT(core < m_numPhysicalCores);
        TURF_ASSERT(hwThread < m_hwThreadsPerCore);
        u32 index = core * m_hwThreadsPerCore + hwThread;
        thread_t thread = mach_thread_self();
        thread_affinity_policy_data_t policyInfo = {(integer_t) index};
        // Note: The following returns KERN_NOT_SUPPORTED on iOS. (Tested on iOS
        // 9.2.)
        kern_return_t result =
            thread_policy_set(thread, THREAD_AFFINITY_POLICY, (thread_policy_t) &policyInfo, THREAD_AFFINITY_POLICY_COUNT);
        return (result == KERN_SUCCESS);
    }
};

} // namespace turf

#endif // TURF_IMPL_AFFINITY_MACH_H
