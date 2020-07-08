cmake_minimum_required(VERSION 3.10)

project(sample_lib)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

vcpkg_add_packages(glfw3 imgui)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(imgui CONFIG REQUIRED)

aux_source_directory(utility UtilitySources)
aux_source_directory(glsl GLSLSources)
aux_source_directory(glad GLADSources)
aux_source_directory(render RenderSources)

add_library(game_lib 
            ${UtilitySources} 
            ${GLSLSources} 
            ${GLADSources}
            ${RenderSources})

target_link_libraries(game_lib ${OPENGL_LIBRARIES} glfw)


