/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <turf/Core.h>
#include <turf/extra/Random.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
//#include <random>

using namespace turf::intTypes;

int main() {
    static const ureg NumBuckets = 65536;
    u64* buckets = new u64[NumBuckets];
    u64* copy = new u64[NumBuckets];
    memset(buckets, 0, sizeof(u64) * NumBuckets);
    turf::extra::Random rand;

    // std::default_random_engine generator;
    // std::uniform_int_distribution<u32> distribution;

    for (;;) {
        for (ureg i = 0; i < 100000; i++) {
            u32 v = rand.next32();
            // u32 v = distribution(generator);
            ureg index = ureg(v / float(4294967297.0f / NumBuckets));
            buckets[index]++;
        }
        for (ureg i = 0; i < NumBuckets; i++)
            copy[i] = buckets[i];
        std::sort(copy, copy + NumBuckets);
        for (ureg i = 0; i < 3; i++)
            printf("%" TURF_U64D " ", copy[i]);
        for (ureg i = NumBuckets - 3; i < NumBuckets; i++)
            printf("%" TURF_U64D " ", copy[i]);
        printf("\n");
    }

    return 0;
}
