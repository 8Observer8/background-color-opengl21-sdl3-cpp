cmake_minimum_required(VERSION 3.20)

project(background_color)
add_executable(app) # WIN32

target_include_directories(app PRIVATE E:/libs/glad-2.0.6/include)

target_sources(app
PRIVATE
    E:/libs/glad-2.0.6/src/glad.c
    main.cpp
)

# This code copies the "SDL3.dll" file to the build directory
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:app> $<TARGET_FILE_DIR:app>
)

find_package(SDL3)
target_link_libraries(app PRIVATE SDL3::SDL3)
target_compile_definitions(app PRIVATE SDL_MAIN_USE_CALLBACKS)
target_link_options(app PRIVATE -static-libgcc -static-libstdc++)
