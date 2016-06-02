cmake_minimum_required (VERSION 2.6)

project (saveBinaryImage)


find_package(OpenCV REQUIRED)

add_executable (saveBinaryImage saveBinaryImage.cpp)

target_link_libraries(saveBinaryImage ${OpenCV_LIBS})