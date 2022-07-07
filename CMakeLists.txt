PROJECT(opencv)
# requirement of cmake version
cmake_minimum_required(VERSION 3.5)

# set the directory of executable files
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# set debug mode
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_definitions(-std=c++11)

# find required opencv
find_package(OpenCV REQUIRED)
# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS})
# name of executable file and path of source file
add_executable(main main.cpp)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(main ${OpenCV_LIBS})