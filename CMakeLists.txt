cmake_minimum_required(VERSION 3.0)
project(MJE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(yaml-cpp CONFIG REQUIRED)
add_executable(MJE main.cpp
        argparse.hpp
)
target_link_libraries(MJE PRIVATE yaml-cpp::yaml-cpp)