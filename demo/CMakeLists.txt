cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

project(delaunay.demo LANGUAGES CXX)

set(project_headers
    include/demo/app.h
    include/demo/window.h)

set(project_sources
    src/app.cpp
    src/main.cpp
    src/window.cpp)

qt5_wrap_cpp(project_moc_sources ${project_headers})

source_group("Source Files//Cpp" ${project_sources})
source_group("Source Files//Moc" ${project_moc_sources})

add_executable(${PROJECT_NAME} ${project_headers} ${project_sources} ${project_moc_sources})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} delaunaypp Qt5::Core Qt5::Gui Qt5::Widgets)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif(WIN32)