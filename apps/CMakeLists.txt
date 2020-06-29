cmake_minimum_required(VERSION 3.10)

project(game_app)

include_directories(${CMAKE_SOURCE_DIR}/sources ${CMAKE_SOURCE_DIR}/includes)

add_executable(game game/main.cpp)

target_link_libraries(game game_lib)