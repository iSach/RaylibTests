cmake_minimum_required(VERSION 3.14)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

#include_directories(external/)

find_package(raylib 2.0 QUIET)

add_executable(untitled1 game.c game.h)
target_link_libraries(untitled1 raylib)

file(COPY "resources/"  DESTINATION "resources/")
