# When we're called from a find module (FindTurf.cmake), as Junction does, TURF_ROOT is already set.
# Otherwise, we're called from Turf's CMakeLists.txt as a standalone project, so define TURF_ROOT here.
if(NOT DEFINED TURF_ROOT)
    get_filename_component(TURF_ROOT ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
endif()

set(TURF_FOUND TRUE)
include("${CMAKE_CURRENT_LIST_DIR}/Macros.cmake")
