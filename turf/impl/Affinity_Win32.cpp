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

#if TURF_TARGET_WIN32

#include <turf/impl/Affinity_Win32.h>
#include <turf/Heap.h>

namespace turf {

Affinity_Win32::Affinity_Win32() {
    m_isAccurate = false;
    m_numPhysicalCores = 0;
    m_numHWThreads = 0;
    for (ureg i = 0; i < MaxHWThreads; i++)
        m_physicalCoreMasks[i] = 0;

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* startProcessorInfo = NULL;
    DWORD length = 0;
    BOOL result = GetLogicalProcessorInformation(NULL, &length);
    if (result == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER && length > 0) {
        startProcessorInfo = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) TURF_HEAP.alloc(length);
        result = GetLogicalProcessorInformation(startProcessorInfo, &length);
        if (result == TRUE) {
            m_isAccurate = true;
            m_numPhysicalCores = 0;
            m_numHWThreads = 0;
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION* endProcessorInfo =
                (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) TURF_PTR_OFFSET(startProcessorInfo, length);
            for (SYSTEM_LOGICAL_PROCESSOR_INFORMATION* processorInfo = startProcessorInfo; processorInfo < endProcessorInfo;
                 processorInfo++) {
                if (processorInfo->Relationship == RelationProcessorCore) {
                    ureg hwt = util::countSetBits(processorInfo->ProcessorMask);
                    if (hwt == 0)
                        m_isAccurate = false;
                    else if (m_numHWThreads + hwt > MaxHWThreads)
                        m_isAccurate = false;
                    else {
                        TURF_ASSERT(m_numPhysicalCores <= m_numHWThreads && m_numHWThreads < MaxHWThreads);
                        m_physicalCoreMasks[m_numPhysicalCores] = processorInfo->ProcessorMask;
                        m_numPhysicalCores++;
                        m_numHWThreads += hwt;
                    }
                }
            }
        }
    }

    TURF_ASSERT(m_numPhysicalCores <= m_numHWThreads);
    if (m_numHWThreads == 0) {
        m_isAccurate = false;
        m_numPhysicalCores = 1;
        m_numHWThreads = 1;
        m_physicalCoreMasks[0] = 1;
    }
}

bool Affinity_Win32::setAffinity(ureg core, ureg hwThread) {
    TURF_ASSERT(hwThread < getNumHWThreadsForCore(core));
    AffinityMask availableMask = m_physicalCoreMasks[core];
    for (AffinityMask checkMask = 1;; checkMask <<= 1) {
        if ((availableMask & checkMask) != 0) {
            if (hwThread-- == 0) {
                DWORD_PTR result = SetThreadAffinityMask(GetCurrentThread(), checkMask);
                return (result != 0);
            }
        }
    }
}

} // namespace turf

#endif // TURF_TARGET_WIN32
