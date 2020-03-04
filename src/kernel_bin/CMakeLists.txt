cmake_minimum_required(VERSION 3.5)
set(libjpeg_path ../../tools/libjpeg/install)
include_directories(${libjpeg_path}/include)
link_directories(${libjpeg_path}/lib)

add_executable(main_demo main.cpp image_loader.cpp $<TARGET_OBJECTS:Vision_Utils>)
target_link_libraries(main_demo jpeg)