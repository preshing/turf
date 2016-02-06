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
#include <turf/Mutex.h>
using namespace turf::intTypes;

//---------------------------------------------------------
// MutexTester
//---------------------------------------------------------
class MutexTester {
private:
    int m_iterationCount;
    turf::Mutex m_mutex;
    int m_value;

public:
    MutexTester() : m_iterationCount(0), m_value(0) {
    }

    void threadFunc(int threadNum) {
        for (int i = 0; i < m_iterationCount; i++) {
            m_mutex.lock();
            m_value++;
            m_mutex.unlock();
        }
    }

    bool test(int threadCount, int iterationCount) {
        m_iterationCount = iterationCount;
        m_value = 0;

        std::vector<std::thread> threads;
        for (int i = 0; i < threadCount; i++)
            threads.emplace_back(&MutexTester::threadFunc, this, i);
        for (std::thread& t : threads)
            t.join();

        return (m_value == threadCount * iterationCount);
    }
};

bool testMutex() {
    MutexTester tester;
    return tester.test(4, 400000);
}
