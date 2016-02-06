/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_TRACE_MEMLOG_H
#define TURF_IMPL_TRACE_MEMLOG_H

#include <turf/Core.h>
#include <turf/TID.h>
#include <turf/Mutex.h>
#include <memory>
#include <turf/Atomic.h>

namespace turf {

//---------------------------------------------------------
// Logs TURF_TRACE events to memory.
// Iterator should only be used after logging is complete.
// Useful for post-mortem debugging and for validating tests.
//---------------------------------------------------------
class Trace_MemLog {
public:
    struct Event {
        turf::TID::TID tid;
        const char* msg;
        uptr param1;
        uptr param2;

        Event() : msg(NULL), param1(0), param2(0) {
        }
    };

private:
    static const ureg MaxNumPages = 4;
    static const ureg EventsPerPage = 16384;

    struct Page {
        Page* next;
        turf::Atomic<sreg> index; // This can exceed EVENTS_PER_PAGE, but it's harmless. Just means page is full.
        Event events[EventsPerPage];

        Page() : next(NULL), index(0) {
        }
    };

    // Store events in a linked list of pages.
    // Mutex is only locked when it's time to allocate a new page.
    turf::Mutex m_mutex;
    Page* m_head;
    turf::Atomic<Page*> m_tail;
    ureg m_numPages; // Protected by m_mutex

    Event* allocateEventFromNewPage();

public:
    Trace_MemLog();
    ~Trace_MemLog();

    void log(const char* msg, uptr param1, uptr param2) {
        turf::signalFenceSeqCst(); // Compiler barrier
        Page* page = m_tail.load(turf::Consume);
        Event* evt;
        sreg index = page->index.fetchAdd(1, turf::Relaxed);
        if (index < EventsPerPage)
            evt = &page->events[index];
        else
            evt = allocateEventFromNewPage(); // Double-checked locking is performed inside here.
        evt->tid = turf::TID::getCurrentThreadID();
        evt->msg = msg;
        evt->param1 = param1;
        evt->param2 = param2;
        turf::signalFenceSeqCst(); // Compiler barrier
    }

    // Iterators are meant to be used only after all logging is complete.
    friend class Iterator;
    class Iterator {
    private:
        Page* m_page;
        sreg m_index;

    public:
        Iterator(Page* p, sreg i) : m_page(p), m_index(i) {
        }

        Iterator& operator++() {
            m_index++;
            if (m_index >= EventsPerPage) {
                Page* next = m_page->next;
                if (next) {
                    m_page = next;
                    m_index = 0;
                } else {
                    m_index = m_page->index.loadNonatomic();
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (m_page != other.m_page) || (m_index != other.m_index);
        }

        const Event& operator*() const {
            return m_page->events[m_index];
        }
    };

    Iterator begin() {
        return Iterator(m_head, 0);
    }

    Iterator end() {
        Page* tail = m_tail.load(turf::Relaxed);
        return Iterator(tail, tail->index.loadNonatomic());
    }

    void dumpStats();
    void dumpEntireLog(const char* path, ureg startPage = 0);

    static Trace_MemLog Instance;
};

} // namespace turf

#define TURF_TRACE_DECLARE(group, count)
#define TURF_TRACE_DEFINE_BEGIN(group, count)
#define TURF_TRACE_DEFINE(desc)
#define TURF_TRACE_DEFINE_END(group, count)
#define TURF_TRACE(group, id, msg, param1, param2) turf::Trace_MemLog::Instance.log(msg, param1, param2)

#endif // TURF_IMPL_TRACE_MEMLOG_H
