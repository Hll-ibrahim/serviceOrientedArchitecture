cmake_minimum_required(VERSION 3.10)
project(serviceOrientedArchitecture)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(serviceOrientedArchitecture main.cpp)
target_link_libraries(serviceOrientedArchitecture ${CURL_LIBRARIES})