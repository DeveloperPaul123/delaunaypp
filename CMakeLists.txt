cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(delaunay)

set(USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_UNIT_TESTS "Turn on to build unit tests for delaunaypp" OFF)
option(BUILD_DEMO_APP "Build a demo app based on Qt5."
)
if(BUILD_UNIT_TESTS)
    enable_testing()
    include(ExternalGoogleTest)
endif(BUILD_UNIT_TESTS)

add_subdirectory(delaunaypp)

if(BUILD_DEMO_APP)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
    add_subdirectory(demo)
endif(BUILD_DEMO_APP)