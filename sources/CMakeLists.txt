cmake_minimum_required(VERSION 3.10)

project(sample_lib)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

vcpkg_add_packages(glfw3)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

aux_source_directory(utility UtilitySources)

add_library(game_lib ${UtilitySources} glad/glad.c)

target_link_libraries(game_lib ${OPENGL_LIBRARIES} glfw)


