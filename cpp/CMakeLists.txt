cmake_minimum_required(VERSION 3.30)

project(TourBox_Linux_Driver LANGUAGES CXX
 DESCRIPTION "A Linux keyboard driver for the Tourbox Neo (TBG_H) "
 HOMEPAGE_URL "https://github.com/raleighlittles/Tourbox_Neo_Linux_Driver")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

add_executable(${PROJECT_NAME} main.cpp)