/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <vector>
#include <thread>
#include <random>
#include <turf/RWLock.h>
#include <turf/Atomic.h>
using namespace turf::intTypes;

//---------------------------------------------------------
// SimpleRWLockTester
// Writes less frequently and only protects a single int.
//---------------------------------------------------------
class SimpleRWLockTester {
private:
    turf::RWLock m_rwLock;
    int m_sharedInt;
    int m_iterationCount;
    turf::Atomic<ureg> m_totalWrites;

public:
    SimpleRWLockTester() : m_iterationCount(0), m_totalWrites(0) {
    }

    void threadFunc(int threadNum) {
        std::random_device rd;
        std::mt19937 randomEngine(rd());
        int writes = 0;
        volatile int accumulator = 0; // Prevent compiler from eliminating this variable

        for (int i = 0; i < m_iterationCount; i++) {
            // Choose randomly whether to read or write.
            if (std::uniform_int_distribution<>(0, 30)(randomEngine) == 0) {
                turf::ExclusiveLockGuard<turf::RWLock> guard(m_rwLock);
                m_sharedInt++;
                writes++;
            } else {
                turf::SharedLockGuard<turf::RWLock> guard(m_rwLock);
                accumulator += m_sharedInt;
            }
        }

        m_totalWrites.fetchAdd(writes, turf::Relaxed);
    }

    bool test(int threadCount, int iterationCount) {
        m_iterationCount = iterationCount;
        m_sharedInt = 0;
        m_totalWrites.store(0, turf::Relaxed);

        // FIXME: Make this use turf::extra::JobDispatcher so that the starts are better synchronized.
        std::vector<std::thread> threads;
        for (int i = 0; i < threadCount; i++)
            threads.emplace_back(&SimpleRWLockTester::threadFunc, this, i);
        for (std::thread& t : threads)
            t.join();

        return (m_sharedInt == m_totalWrites.load(turf::Relaxed));
    }
};

bool testRWLockSimple() {
    SimpleRWLockTester tester;
    return tester.test(4, 2000000);
}
