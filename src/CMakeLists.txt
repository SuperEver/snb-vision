cmake_minimum_required(VERSION 3.5)
project(snowboy_vision_lib)
set(CMAKE_CXX_STANDARD 11)

include(cmake/install.cmake)

include_directories(3rdparty/ncnn/install/include)
find_library(LIB_NCNN libncnn.a 3rdparty/ncnn/install/lib)

add_subdirectory(utils)
add_subdirectory(kernel)
add_subdirectory(kernel_bin)
#add_subdirectory(kernel_sdk)


set(HEADERS kernel/UltraFace.h)
install_header(${HEADERS})