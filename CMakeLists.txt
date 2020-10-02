cmake_minimum_required(VERSION 3.18)
project(star-conflict)
project(albion-online)

find_package( OpenCV REQUIRED )
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})

set(CMAKE_CXX_STANDARD 20)

#include_directories(.)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${X11_INCLUDE_DIR})

add_executable(star-conflict star-conflict/main.cpp)
target_link_libraries(star-conflict ${OpenCV_LIBS} )

add_executable(albion-online albion-online/main.cpp)
target_link_libraries(albion-online ${OpenCV_LIBS} )
