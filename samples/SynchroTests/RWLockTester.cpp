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
#include <turf/Mutex.h>
#include <string>
#include <thread>
#include <random>
#include <turf/RWLock.h>
#include <turf/Atomic.h>
using namespace turf::intTypes;

//---------------------------------------------------------
// RWLockTester
//---------------------------------------------------------
class RWLockTester {
private:
    static const int SharedArraySize = 8;
    int m_shared[SharedArraySize];
    turf::RWLock m_rwLock;
    int m_iterationCount;
    turf::Atomic<sreg> m_success;

public:
    RWLockTester() : m_iterationCount(0), m_success(0) {
    }

    void threadFunc(int threadNum) {
        std::random_device rd;
        std::mt19937 randomEngine(rd());

        for (int i = 0; i < m_iterationCount; i++) {
            // Choose randomly whether to read or write.
            if (std::uniform_int_distribution<>(0, 3)(randomEngine) == 0) {
                // Write an incrementing sequence of numbers (backwards).
                int value = std::uniform_int_distribution<>()(randomEngine);
                turf::ExclusiveLockGuard<turf::RWLock> guard(m_rwLock);
                for (int j = SharedArraySize - 1; j >= 0; j--) {
                    m_shared[j] = value--;
                }
            } else {
                // Check that the sequence of numbers is incrementing.
                bool ok = true;
                {
                    turf::SharedLockGuard<turf::RWLock> guard(m_rwLock);
                    int value = m_shared[0];
                    for (int j = 1; j < SharedArraySize; j++) {
                        ok = ok && (++value == m_shared[j]);
                    }
                }
                if (!ok) {
                    m_success.store(0, turf::Relaxed);
                }
            }
        }
    }

    bool test(int threadCount, int iterationCount) {
        m_iterationCount = iterationCount;
        for (int j = 0; j < SharedArraySize; j++)
            m_shared[j] = j;
        m_success.storeNonatomic(1);

        // FIXME: Make this use turf::extra::JobDispatcher so that the starts are better synchronized.
        std::vector<std::thread> threads;
        for (int i = 0; i < threadCount; i++)
            threads.emplace_back(&RWLockTester::threadFunc, this, i);
        for (std::thread& t : threads)
            t.join();

        return m_success.loadNonatomic() != 0;
    }
};

bool testRWLock() {
    RWLockTester tester;
    return tester.test(4, 1000000);
}
