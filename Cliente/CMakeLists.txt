cmake_minimum_required(VERSION 4.0)
project(cliente C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_executable(cliente main.c)
target_link_libraries(cliente ${GTK4_LIBRARIES})