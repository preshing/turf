/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <turf/CPUTimer.h>
#include <iostream>

//---------------------------------------------------------
// List of tests
//---------------------------------------------------------
struct TestInfo {
    const char* name;
    bool (*testFunc)();
};

bool testMutex();
bool testRecursiveMutex();
bool testRWLock();
bool testRWLockSimple();

// clang-format off
#define ADD_TEST(name) {#name, name},
TestInfo g_tests[] = {
    ADD_TEST(testMutex)
    ADD_TEST(testRecursiveMutex)
    ADD_TEST(testRWLock)
    ADD_TEST(testRWLockSimple) 
};
// clang-format on

//---------------------------------------------------------
// main
//---------------------------------------------------------
int main() {
    bool allTestsPassed = true;

    for (const TestInfo& test : g_tests) {
        std::cout << "Running " << test.name << "...";

        turf::CPUTimer::Point start = turf::CPUTimer::get();
        bool result = test.testFunc();
        turf::CPUTimer::Point end = turf::CPUTimer::get();

        float millis = turf::CPUTimer::Converter().toSeconds(end - start) * 1000.f;
        std::cout << " " << (result ? "passed" : "***FAILED***") << " in " << millis << " ms\n";
        allTestsPassed = allTestsPassed && result;
    }

    return allTestsPassed ? 0 : 1;
}
