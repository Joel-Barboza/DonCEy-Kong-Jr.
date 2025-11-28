cmake_minimum_required(VERSION 3.10)
project(ClienteTCP C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
set(SDLTTF_VENDORED ON)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)


add_executable(cliente_tcp main.c sdl3.c
        socket_thread.h
        socket_thread.c
        widgets/button.h
        widgets/button.c
        widgets/input_field.h
        widgets/input_field.c
        cjson/cJSON.c
        cjson/cJSON.h
        cjson/cJSON_Utils.c
        cjson/cJSON_Utils.h
        widgets/fruit.c
        widgets/fruit.h
        constants.h
        widgets/crocodile.c
        widgets/crocodile.h
        constants.c)

if(WIN32)
    target_link_libraries(cliente_tcp PRIVATE ws2_32 SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
else()
    target_link_libraries(cliente_tcp PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
endif()

# Copy DLLs after build (for Windows)
add_custom_command(TARGET cliente_tcp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:SDL3::SDL3>/SDL3.dll"
        "$<TARGET_FILE_DIR:SDL3_ttf::SDL3_ttf>/SDL3_ttf.dll"
        "$<TARGET_FILE_DIR:SDL3_image::SDL3_image>/SDL3_image.dll"
        $<TARGET_FILE_DIR:cliente_tcp>
)

add_custom_command(TARGET cliente_tcp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "${CMAKE_CURRENT_BINARY_DIR}/resources"
)

