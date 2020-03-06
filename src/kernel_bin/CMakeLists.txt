cmake_minimum_required(VERSION 3.5)

add_executable(image_loader_test image_loader_test.cpp image_loader.cpp $<TARGET_OBJECTS:Vision_Utils>)
set(OpenCV_DIR ../3rdparty/opencv/install/share/OpenCV)
find_package(OpenCV REQUIRED)
target_link_libraries(image_loader_test ${OpenCV_LIBS})

add_executable(ultraface_test UltraFace_test.cpp $<TARGET_OBJECTS:Vision_KERNEL> $<TARGET_OBJECTS:Vision_Utils>)
target_link_libraries(ultraface_test ${OpenCV_LIBS} ${LIB_NCNN})

install_executable(ultraface_test)