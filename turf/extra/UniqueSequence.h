/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_UNIQUESEQUENCE_H
#define TURF_EXTRA_UNIQUESEQUENCE_H

#include <turf/Core.h>
#include <turf/extra/Random.h>

namespace turf {
namespace extra {

struct UniqueSequence {
    u32 curIndex;
    u32 param0;
    u32 param1;

    UniqueSequence(u32 startIndex = 0, u32 param0 = 0x7012fe08, u32 param1 = 0x5bf03635)
        : curIndex(startIndex), param0(param0), param1(param1) {
    }

    UniqueSequence(Random& seed) : UniqueSequence(seed.next32(), seed.next32()) {
    }

    static u32 permute(u32 x) {
        static const u32 prime = 4294967291;
        if (x >= prime)
            return x;
        u32 residue = ((u64) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }

    u32 next() {
        return permute((permute(curIndex++) + param0) ^ param1);
    }
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_UNIQUESEQUENCE_H
