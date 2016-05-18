/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <memory>
#include <turf/impl/Trace_MemLog.h>
#include <stdio.h>
#include <turf/Util.h>

namespace turf {

Trace_MemLog Trace_MemLog::Instance;

Trace_MemLog::Trace_MemLog() : m_head(new Page), m_tail(m_head), m_numPages(1) {
}

Trace_MemLog::~Trace_MemLog() {
    // Pages are not cleaned up
}

Trace_MemLog::Event* Trace_MemLog::allocateEventFromNewPage() {
    turf::LockGuard<turf::Mutex> lock(m_mutex);
    // Double-checked locking:
    // Check again whether the current page is full. Another thread may have called
    // allocateEventFromNewPage and created a new page by the time we get take the lock.
    Page* oldTail = m_tail.loadNonatomic();
    if (oldTail->index.load(turf::Relaxed) < EventsPerPage) {
        sreg index = oldTail->index.fetchAdd(1, turf::Relaxed);
        // Yes! We got a slot on this page.
        if (index < EventsPerPage)
            return &oldTail->events[index];
    }

    // OK, we're definitely out of space. It's up to us to allocate a new page.
    Page* newTail = new Page;
    // Reserve the first slot.
    newTail->index.storeNonatomic(1);
    // A plain non-atomic move to oldTail->next is fine because there are no other writers here,
    // and nobody is supposed to read the logged contents until all logging is complete.
    oldTail->next = newTail;
    // m_tail must be written atomically because it is read concurrently from other threads.
    // We also use release/consume semantics so that its constructed contents are visible to other threads.
    // Again, very much like the double-checked locking pattern.
    m_tail.store(newTail, turf::Release);
    if (m_numPages >= MaxNumPages) {
        Page* oldHead = m_head;
        m_head = oldHead->next;
        delete oldHead;
    } else {
        m_numPages++;
    }

    // Return the reserved slot.
    return &newTail->events[0];
}

void Trace_MemLog::dumpStats() {
    ureg numEvents = 0;
    {
        turf::LockGuard<turf::Mutex> lock(m_mutex);
        numEvents = (m_numPages - 1) * EventsPerPage + m_tail.load(turf::Consume)->index.load(turf::Relaxed);
    }
    printf("%" TURF_UREGD " events logged\n", numEvents);
}

void Trace_MemLog::dumpEntireLog(const char* path, ureg startPage) {
    turf::LockGuard<turf::Mutex> lock(m_mutex);
    FILE* f = fopen(path, "w");
    for (Page* page = m_head; page; page = page->next) {
        if (startPage > 0) {
            startPage--;
            continue;
        }
        ureg limit = turf::util::min<ureg>(EventsPerPage, page->index.load(turf::Relaxed));
        for (ureg i = 0; i < limit; i++) {
            const Event& evt = page->events[i];
            fprintf(f, "[%" TURF_U64X "] %" TURF_UPTRX " %" TURF_UPTRX " %s\n", (u64) evt.tid, evt.param1, evt.param2, evt.msg);
        }
    }
    fclose(f);
}

} // namespace turf
