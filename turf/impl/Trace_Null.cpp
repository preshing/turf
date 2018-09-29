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

#if !TURF_DLL_IMPORTING

#include <memory>
#include <turf/impl/Trace_Null.h>

namespace turf {

Trace_Null Trace_Null::Instance;

} // namespace turf

#endif // !TURF_DLL_IMPORTING
