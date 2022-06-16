cmake_minimum_required(VERSION 2.8)
project(test_ws)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-std=c++14 -O2")

find_package(Ceres REQUIRED)

include_directories(
    include
    "/usr/include/eigen3"
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CERES_LIBRARIES})