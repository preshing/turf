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
#include <turf/impl/Trace_Counters.h>
#include <stdio.h>

namespace turf {

Trace_Counters Trace_Counters::Instance; // Zero-initialized

void Trace_Counters::addGroup(TraceGroup* group) {
    TraceGroup* oldHead = m_firstGroup.load(turf::Relaxed);
    do {
        group->m_next = oldHead;
    } while (!m_firstGroup.compareExchangeWeak(oldHead, group, turf::Relaxed, turf::Relaxed));
}

void Trace_Counters::dumpStats() {
    TraceGroup* group = m_firstGroup.load(turf::Relaxed);
    for (; group; group = group->m_next) {
        group->dumpIfUsed();
    }
}

void TraceGroup::dump() {
    printf("--------------- %s\n", m_name);
    for (ureg i = 0; i < m_numCounters; i++) {
        printf("%12" TURF_U32D ": %s\n", m_counters[i].count.load(turf::Relaxed), m_counters[i].str);
    }
}

void TraceGroup::dumpIfUsed() {
    for (ureg i = 0; i < m_numCounters; i++) {
        if (m_counters[i].count.load(turf::Relaxed)) {
            dump();
            break;
        }
    }
}

} // namespace turf
