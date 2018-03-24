cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(delaunaypp LANGUAGES CXX)

set(project_headers
    include/delaunaypp/delaunay.h
    include/delaunaypp/edge.h
    include/delaunaypp/point.h
    include/delaunaypp/pointaccessor.h)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} 
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_auto_type)

if(MSVC)
    add_custom_target(${PROJECT_NAME}.headers SOURCES ${project_headers})
endif()

if(BUILD_UNIT_TESTS)
    message(STATUS "Adding unit tests.")
    set(test_sources 
        test/delaunaypp_tests.cpp)
    
    set(project_test_name "${PROJECT_NAME}.tests")
    add_executable(${project_test_name} test/delaunaypp_tests.cpp)

    target_link_libraries(${project_test_name} 
        delaunaypp
        googletest 
        googletest_main)
    add_test(${project_test_name} ${project_test_name})
endif(BUILD_UNIT_TESTS)