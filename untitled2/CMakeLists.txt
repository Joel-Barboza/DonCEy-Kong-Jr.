cmake_minimum_required(VERSION 3.10)
project(ClienteTCP C)

set(CMAKE_C_STANDARD 11)

add_executable(cliente_tcp main.c)

if(WIN32)
    target_link_libraries(cliente_tcp ws2_32)
endif()