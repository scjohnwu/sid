cmake_minimum_required(VERSION 3.10)

project(game_app)

vcpkg_add_packages(glfw3)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(game game/main.cpp)

target_link_libraries(game ${OPENGL_LIBRARIES} glfw)
