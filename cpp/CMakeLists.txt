cmake_minimum_required(VERSION 3.22)
project(TourBox_Linux_Driver DESCRIPTION "TODO")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -Wpedantic -Werror")

add_executable(${PROJECT_NAME} main.cpp)