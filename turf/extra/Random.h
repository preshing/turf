/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_RANDOM_H
#define TURF_EXTRA_RANDOM_H

#include <turf/Core.h>
#include <turf/Atomic.h>

namespace turf {
namespace extra {

//-------------------------------------
//  xorshift128+ generator seeded using misc. information from the environment.
//  http://xorshift.di.unimi.it/
//-------------------------------------
class Random {
private:
    u64 s[2];

public:
    Random();
    u64 next64();
    u32 next32() {
        return (u32) next64();
    }
    u16 next16() {
        return (u16) next64();
    }
    u8 next8() {
        return (u8) next64();
    }
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_RANDOM_H
