cmake_minimum_required(VERSION 3.10)

project(sample_lib)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

vcpkg_add_packages(glfw3)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(game_lib utility/window.cpp)

target_link_libraries(game_lib ${OPENGL_LIBRARIES} glfw)

