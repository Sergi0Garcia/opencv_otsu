cmake_minimum_required(VERSION 2.8)
project(opencv_otsu)

find_package(OpenCV REQUIRED)

include_directories(
include
${OpenCV_INCLUDE_DIRS}
)

add_executable(
	otsu
	src/main.cpp
	src/otsu.cpp
	)

target_link_libraries(otsu ${OpenCV_LIBS})