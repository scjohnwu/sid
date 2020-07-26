cmake_minimum_required(VERSION 3.10)

project(viewer_app)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

find_package(imgui REQUIRED)

aux_source_directory(viewer/gui GUISources)

add_executable(viewer viewer/main.cpp ${GUISources})
target_link_libraries(viewer engine_lib imgui::imgui)

add_custom_command(TARGET viewer PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders/fragment $<TARGET_FILE_DIR:viewer>)
            
add_custom_command(TARGET viewer PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders/vertex $<TARGET_FILE_DIR:viewer>)
            
add_custom_command(TARGET viewer PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:viewer>)

add_custom_command(TARGET viewer PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:viewer>)