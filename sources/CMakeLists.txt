cmake_minimum_required(VERSION 3.10)

project(sample_lib)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

vcpkg_add_packages(glfw3 imgui spdlog globjects glbinding glm tinyobjloader)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(globjects REQUIRED)
find_package(glbinding REQUIRED)
find_package(glm REQUIRED)
find_package(tinyobjloader REQUIRED)

aux_source_directory(utility UtilitySources)
aux_source_directory(gui GUISources)
aux_source_directory(render RenderSources)
aux_source_directory(scene SceneSources)

add_library(game_lib 
            ${UtilitySources} 
            ${GUISources}
            ${RenderSources}
            ${SceneSources})

target_link_libraries(game_lib 
                      ${OPENGL_LIBRARIES} 
                      spdlog::spdlog 
                      globjects::globjects 
                      glbinding::glbinding
                      tinyobjloader::tinyobjloader
                      glm
                      glfw)


