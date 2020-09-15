cmake_minimum_required(VERSION 3.18)
project(PlayBot)

find_package( OpenCV REQUIRED )
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})

set(CMAKE_CXX_STANDARD 20)

#include_directories(.)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${X11_INCLUDE_DIR})

add_executable(PlayBot main.cpp)
target_link_libraries( PlayBot ${OpenCV_LIBS} )