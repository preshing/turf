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
#include <random>
#include <thread>
#include <turf/Assert.h>
using namespace turf::intTypes;

//---------------------------------------------------------
// RecursiveMutexTester
//---------------------------------------------------------
class RecursiveMutexTester {
private:
    struct ThreadStats {
        ureg iterations;
        ureg workUnitsComplete;
        ureg amountIncremented;

        ThreadStats() {
            iterations = 0;
            workUnitsComplete = 0;
            amountIncremented = 0;
        }

        ThreadStats& operator+=(const ThreadStats& other) {
            iterations += other.iterations;
            workUnitsComplete += other.workUnitsComplete;
            amountIncremented += other.amountIncremented;
            return *this;
        }
    };

    ureg m_iterationCount;
    turf::Mutex m_mutex;
    ureg m_value;
    std::vector<ThreadStats> m_threadStats;

public:
    RecursiveMutexTester() : m_iterationCount(0), m_value(0) {
    }

    void threadFunc(ureg threadNum) {
        std::random_device rd;
        std::mt19937 randomEngine(rd());
        ThreadStats localStats;
        ureg lockCount = 0;
        ureg lastValue = 0;

        for (ureg i = 0; i < m_iterationCount; i++) {
            localStats.iterations++;

            // Do a random amount of work.
            ureg workUnits = std::uniform_int_distribution<>(0, 3)(randomEngine);
            for (ureg j = 1; j < workUnits; j++)
                randomEngine(); // Do one work unit
            localStats.workUnitsComplete += workUnits;

            // Consistency check.
            if (lockCount > 0) {
                TURF_ASSERT(m_value == lastValue);
            }

            // Decide what the new lock count should be in the range [0, 4), biased towards low numbers.
            float f = std::uniform_real_distribution<float>(0.f, 1.f)(randomEngine);
            ureg desiredLockCount = (ureg)(f * f * 4);

            // Perform unlocks, if any.
            while (lockCount > desiredLockCount) {
                m_mutex.unlock();
                lockCount--;
            }

            // Perform locks, if any.
            bool useTryLock = (std::uniform_int_distribution<>(0, 1)(randomEngine) == 0);
            while (lockCount < desiredLockCount) {
                if (useTryLock) {
                    if (!m_mutex.tryLock())
                        break;
                } else {
                    m_mutex.lock();
                }
                lockCount++;
            }

            // If locked, increment counter.
            if (lockCount > 0) {
                TURF_ASSERT((m_value - lastValue) >= 0);
                m_value += threadNum + 1;
                lastValue = m_value;
                localStats.amountIncremented += threadNum + 1;
            }
        }

        // Release Lock if still holding it.
        while (lockCount > 0) {
            m_mutex.unlock();
            lockCount--;
        }

        // Copy statistics.
        m_threadStats[threadNum] = localStats;
    }

    bool test(ureg threadCount, ureg iterationCount) {
        m_iterationCount = iterationCount;
        m_value = 0;
        m_threadStats.resize(threadCount);

        std::vector<std::thread> threads;
        for (ureg i = 0; i < threadCount; i++)
            threads.emplace_back(&RecursiveMutexTester::threadFunc, this, i);
        for (std::thread& t : threads)
            t.join();

        ThreadStats totalStats;
        for (const ThreadStats& s : m_threadStats)
            totalStats += s;
        return (m_value == totalStats.amountIncremented);
    }
};

bool testRecursiveMutex() {
    RecursiveMutexTester tester;
    return tester.test(4, 100000);
}
