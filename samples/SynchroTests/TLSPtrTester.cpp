/*------------------------------------------------------------------------
Turf: Configurable C++ platform adapter
Copyright (c) 2016 Jeff Preshing and Bret Alfieri

Distributed under the Simplified BSD License.
Original location: https://github.com/preshing/turf

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <vector>
#include <turf/Thread.h>
#include <turf/TID.h>
#include <turf/TLSPtr.h>

#include <stdlib.h>

using namespace turf::intTypes;

//---------------------------------------------------------
// TSLPtr Tester
//---------------------------------------------------------
class TLSPtrTester {
private:
    turf::TLSPtr<int> m_tlsPtr;
    std::vector<int> m_tlsValuesByThreadNum;

    struct TestStruct
    {
        int value;
    };

public:

    struct ThreadParam {
        int threadNum;
        TLSPtrTester* pTester;
    };

    static turf::Thread::ReturnType TURF_THREAD_STARTCALL threadFunc(void* param) {
        ThreadParam* threadParam = static_cast<ThreadParam*>(param);

        int threadNum = threadParam->threadNum;
        turf::TLSPtr<int>& tlsPtr = threadParam->pTester->m_tlsPtr;

        tlsPtr.setData(new int(threadNum));

        turf::Thread::sleepMillis(rand() % 100);

        threadParam->pTester->m_tlsValuesByThreadNum[threadNum] = *tlsPtr;

        delete threadParam;
        return 0;
    }

    bool testInterface() {
        const int TEST_VALUE = 5;

        {
            turf::TLSPtr<TestStruct> tlsPtr;
        }
        {
            TestStruct* data = new TestStruct;
            data->value = TEST_VALUE;

            turf::TLSPtr<TestStruct> tlsPtr;
            tlsPtr.setData(data);

            if (data != tlsPtr.getData())
                return false;

            if (tlsPtr->value != TEST_VALUE)
                return false;

            if ((*tlsPtr).value != TEST_VALUE)
                return false;
        }

        return true;
    }

    bool testFunctionality(int threadCount) {
        m_tlsValuesByThreadNum.resize(threadCount);

        std::vector<turf::Thread> threads(threadCount);
        for (int ii = 0; ii < threadCount; ii++) {
            ThreadParam* param = new ThreadParam;
            param->threadNum = ii;
            param->pTester = this;
            threads[ii].run(&TLSPtrTester::threadFunc, param);
        }
        for (int ii = 0; ii < threadCount; ii++) {
            threads[ii].join();
        }

        for (int ii = 0; ii < threadCount; ii++) {
            if (m_tlsValuesByThreadNum[ii] != ii)
            {
                return false;
            }
        }
        return true;
    }
};

bool testTLSPtr() {
    TLSPtrTester tester;

    if (!tester.testInterface())
        return false;

    if (!tester.testFunctionality(200))
        return false;

    return true;
}
