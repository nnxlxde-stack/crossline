cmake_minimum_required(VERSION 4.2.3)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "std")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests main.cpp)

target_sources(tests PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    light_test.cppm
    crossline_ui/crossline_ui_reactive.cppm
    crossline_ui/crossline_ui_reactive_tests.cppm
)