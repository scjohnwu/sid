cmake_minimum_required(VERSION 3.10)

project(game_app)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

find_package(imgui REQUIRED)

aux_source_directory(game/gui GUISources)

add_executable(game game/main.cpp ${GUISources})
target_link_libraries(game game_lib imgui::imgui)

add_custom_command(TARGET game PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders/fragment $<TARGET_FILE_DIR:game>)
            
add_custom_command(TARGET game PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders/vertex $<TARGET_FILE_DIR:game>)