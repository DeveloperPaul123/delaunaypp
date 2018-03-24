# This file will download and configure google testing
cmake_minimum_required(VERSION 2.8.8)

include(cmake/Externals.cmake)

ExternalProject_Add(GoogleTestExternal
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "718fd88d8f145c63b8cc134cf8fed92743cc112f"
    CMAKE_ARGS -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
               -Dgtest_force_shared_crt=ON
               -DBUILD_GTEST=ON
    PREFIX "${EXTERNAL_BUILD_DIR}"
	# Disable install step
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(GoogleTestExternal source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)

# Specify MainTest's link libraries
ExternalProject_Get_Property(GoogleTestExternal binary_dir)
set(GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest)

add_library(googletest STATIC IMPORTED)
add_library(googletest_main STATIC IMPORTED)
set_target_properties(googletest PROPERTIES
    IMPORTED_LOCATION_DEBUG ${GTEST_LIBS_DIR}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtestd${CMAKE_FIND_LIBRARY_SUFFIXES}
    IMPORTED_LOCATION_RELEASE ${GTEST_LIBS_DIR}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}    
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})

set_target_properties(googletest_main PROPERTIES
    IMPORTED_LOCATION_DEBUG ${GTEST_LIBS_DIR}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_maind${CMAKE_FIND_LIBRARY_SUFFIXES}
    IMPORTED_LOCATION_RELEASE ${GTEST_LIBS_DIR}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})

add_dependencies(googletest GoogleTestExternal)
add_dependencies(googletest_main googletest)