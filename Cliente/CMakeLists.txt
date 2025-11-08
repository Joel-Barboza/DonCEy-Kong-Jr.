cmake_minimum_required(VERSION 3.10)
project(ClienteTCP C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(cliente_tcp main.c sdl3.c)

if(WIN32)
    target_link_libraries(cliente_tcp PRIVATE ws2_32 SDL3::SDL3)
else()
    target_link_libraries(cliente_tcp PRIVATE SDL3::SDL3)
endif()