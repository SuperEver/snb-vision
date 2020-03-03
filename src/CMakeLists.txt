cmake_minimum_required(VERSION 3.5)
project(snowboy_vision_lib)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(kernel)

add_executable(main_demo ./main.cpp $<TARGET_OBJECTS:Vision_External> $<TARGET_OBJECTS:Vision_Utils>)
