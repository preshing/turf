#---------------------------------------------------------------------------
# This script is included from the CMakeLists.txt (listfile) of sample applications.
#---------------------------------------------------------------------------

# Were we included from the root listfile?
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Yes, it's the root.
    include("${CMAKE_CURRENT_LIST_DIR}/../cmake/Macros.cmake")
    ApplyTurfBuildSettings()
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/.." turf)
endif()

macro(AddSampleTargetFromSources SAMPLE_FILES)
    # Define executable target.
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.mycompany.\${PRODUCT_NAME:identifier}")
    add_executable(${SAMPLE_NAME} MACOSX_BUNDLE ${SAMPLE_FILES})
    set_target_properties(${SAMPLE_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    set_target_properties(${SAMPLE_NAME} PROPERTIES FOLDER samples)

    # Set include dirs and libraries
    include_directories(${TURF_ALL_INCLUDE_DIRS})
    target_link_libraries(${SAMPLE_NAME} ${TURF_ALL_LIBRARIES})
endmacro()

macro(AddSampleTarget)
    GetFilesWithSourceGroups(GLOB SAMPLE_FILES ${CMAKE_CURRENT_SOURCE_DIR} *)
    AddSampleTargetFromSources("${SAMPLE_FILES}")
endmacro()