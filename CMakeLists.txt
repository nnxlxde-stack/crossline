cmake_minimum_required(VERSION 4.2.3)

project(crossline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

add_executable(crossline core/main.cpp)