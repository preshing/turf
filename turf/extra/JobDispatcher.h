/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_JOBDISPATCHER_H
#define TURF_EXTRA_JOBDISPATCHER_H

#include <turf/Core.h>
#include <turf/Assert.h>
#include <turf/Affinity.h>
#include <turf/extra/SpinKicker.h>
#include <vector>

namespace turf {
namespace extra {

class JobDispatcher {
private:
    struct WorkerThread {
        JobDispatcher* dispatcher;
        ureg threadIndex;
        turf::Thread thread;
        bool mustExit;

        WorkerThread(JobDispatcher* dispatcher, ureg threadIndex)
            : dispatcher(dispatcher), threadIndex(threadIndex), mustExit(false) {
        }
    };

    typedef void Action(void*, ureg);

    turf::Affinity m_affinity;
    bool m_useAffinities;
    std::vector<WorkerThread*> m_threads;
    sreg m_threadFilter;
    Action* m_action;
    void* m_param;
    turf::extra::SpinKicker m_startGate;
    turf::extra::SpinKicker m_endGate;

    void threadRun(WorkerThread* thread) {
        if (m_useAffinities)
            m_affinity.setAffinity(thread->threadIndex, 0);
        for (;;) {
            m_startGate.waitForKick();
            // Need to fetch the WorkerThread struct on each kick since
            // the array can resize between kicks.
            if (thread->mustExit)
                break;
            if (m_threadFilter < 0 || m_threadFilter == thread->threadIndex)
                m_action(m_param, thread->threadIndex);
            m_endGate.waitForKick();
        }
    }

    static turf::Thread::ReturnType TURF_THREAD_STARTCALL threadStart(void* param) {
        WorkerThread* thread = (WorkerThread*) param;
        thread->dispatcher->threadRun(thread);
        return 0;
    }

    static void noAction(void*, ureg) {
    }

    void resetAction() {
        m_threadFilter = -1;
        m_action = noAction;
        m_param = NULL;
    }

public:
    JobDispatcher(ureg numThreads = 0) : m_useAffinities(numThreads == 0) {
        m_threads.push_back(new WorkerThread(this, 0));
        if (m_useAffinities)
            m_affinity.setAffinity(0, 0);
        resetAction();
        if (numThreads > 0)
            setNumSpawnedThreads(numThreads);
    }

    ~JobDispatcher() {
        setNumSpawnedThreads(1);
        // FIXME: Reset affinity to default.
    }

    ureg getNumPhysicalCores() const {
        return m_affinity.getNumPhysicalCores();
    }

    void setNumSpawnedThreads(ureg numThreads) {
        TURF_ASSERT(numThreads > 0);
        if (m_useAffinities)
            TURF_ASSERT(numThreads <= m_affinity.getNumPhysicalCores());
        ureg oldNumThreads = m_threads.size();
        if (numThreads < oldNumThreads) {
            resetAction();
            for (ureg t = numThreads; t < oldNumThreads; t++) {
                m_threads[t]->mustExit = true;
            }
            m_startGate.kick(oldNumThreads - 1);
            for (ureg t = numThreads; t < oldNumThreads; t++) {
                m_threads[t]->thread.join();
                delete m_threads[t];
            }
            m_threads.resize(numThreads);
            m_endGate.kick(numThreads - 1);
        } else {
            for (; oldNumThreads < numThreads; oldNumThreads++) {
                WorkerThread* thread = new WorkerThread(this, oldNumThreads);
                m_threads.push_back(thread);
                thread->thread.run(threadStart, thread);
            }
        }
    }

    template <class T>
    void kickOne(ureg threadIndex, void (T::*pmf)(), T& target) {
        // Define the action
        struct Closure {
            void (T::*pmf)();
            T* target;
            static void thunk(void* param, ureg) {
                Closure* self = (Closure*) param;
                TURF_CALL_MEMBER (*self->target, self->pmf)();
            }
        };
        Closure closure = {pmf, &target};
        m_threadFilter = threadIndex;
        m_action = Closure::thunk;
        m_param = &closure;
        // Kick the threads
        if (threadIndex >= m_threads.size())
            setNumSpawnedThreads(threadIndex + 1);
        m_startGate.kick(m_threads.size() - 1);
        if (threadIndex == 0)
            m_action(m_param, 0);
        m_endGate.kick(m_threads.size() - 1);
        resetAction();
    }

    template <class T>
    void kickMulti(void (T::*pmf)(), T* targets, ureg numTargets) {
        // Define the action
        struct Closure {
            void (T::*pmf)();
            T* targets;
            static void thunk(void* param, ureg index) {
                Closure* self = (Closure*) param;
                TURF_CALL_MEMBER(self->targets[index], self->pmf)();
            }
        };
        Closure closure = {pmf, targets};
        m_action = Closure::thunk;
        m_param = &closure;
        // Kick the threads
        setNumSpawnedThreads(numTargets);
        m_startGate.kick(m_threads.size() - 1);
        m_action(m_param, 0);
        m_endGate.kick(m_threads.size() - 1);
        resetAction();
    }

    template <class T>
    void kick(void (T::*pmf)(ureg), T& target) {
        // Define the action
        struct Closure {
            void (T::*pmf)(ureg);
            T* target;
            static void thunk(void* param, ureg index) {
                Closure* self = (Closure*) param;
                TURF_CALL_MEMBER (*self->target, self->pmf)(index);
            }
        };
        Closure closure = {pmf, &target};
        m_action = Closure::thunk;
        m_param = &closure;
        // Kick the threads
        m_startGate.kick(m_threads.size() - 1);
        m_action(m_param, 0);
        m_endGate.kick(m_threads.size() - 1);
        resetAction();
    }

    void kick(void (*func)(ureg)) {
        // Define the action
        struct Closure {
            void (*func)(ureg);
            static void thunk(void* param, ureg index) {
                Closure* self = (Closure*) param;
                self->func(index);
            }
        };
        Closure closure = {func};
        m_action = Closure::thunk;
        m_param = &closure;
        // Kick the threads
        m_startGate.kick(m_threads.size() - 1);
        m_action(m_param, 0);
        m_endGate.kick(m_threads.size() - 1);
        resetAction();
    }
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_JOBDISPATCHER_H
