cmake_minimum_required(VERSION 3.15)
project(first_person_shooter_lol)

ADD_SUBDIRECTORY(3rdparty/raylib)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

set(CMAKE_C_STANDARD 11)

ADD_SUBDIRECTORY(3rdparty/entt)

file (GLOB_RECURSE C_FILES src/*.cpp)
file (GLOB_RECURSE H_FILES src/*.h)
set (SOURCE_FILES ${C_FILES} ${H_FILES})
add_executable(first_person_shooter_lol ${C_FILES})

target_link_libraries(first_person_shooter_lol raylib)
