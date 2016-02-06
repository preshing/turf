/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TRACE_COUNTERS_H
#define TURF_IMPL_TRACE_COUNTERS_H

#include <turf/Core.h>
#include <turf/Atomic.h>

namespace turf {

class TraceGroup;

class Trace_Counters {
private:
    turf::Atomic<TraceGroup*> m_firstGroup;

public:
    void addGroup(TraceGroup* group);
    void dumpStats();

    static Trace_Counters Instance; // Zero-initialized
};

class TraceGroup {
public:
    struct Counter {
        turf::Atomic<u32> count;
        const char* str;
    };

private:
    friend class Trace_Counters;
    const char* m_name;
    Counter* m_counters;
    ureg m_numCounters;
    TraceGroup* m_next;

public:
    TraceGroup(const char* name, Counter* counters, ureg numCounters)
        : m_name(name), m_counters(counters), m_numCounters(numCounters), m_next(NULL) {
        Trace_Counters::Instance.addGroup(this);
    }
    void dump();
    void dumpIfUsed();
};

// clang-format off
#define TURF_TRACE_DECLARE(group, count)      extern turf::TraceGroup::Counter Trace_##group[count];
#define TURF_TRACE_DEFINE_BEGIN(group, count) turf::TraceGroup::Counter Trace_##group[count] = {
#define TURF_TRACE_DEFINE(desc)                   { 0, desc },
#define TURF_TRACE_DEFINE_END(group, count)   }; \
                                              turf::TraceGroup TraceGroup_##group(#group, Trace_##group, count);
#define TURF_TRACE(group, index, msg, param1, param2) \
    Trace_##group[index].count.fetchAdd(1, turf::Relaxed)
// clang-format on

} // namespace turf

#endif // TURF_IMPL_TRACE_COUNTERS_H
