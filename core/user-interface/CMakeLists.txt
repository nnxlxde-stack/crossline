cmake_minimum_required(VERSION 4.2.3)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "std")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

project(crossline-user-interface)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(crossline-user-interface lib_main.cpp)

target_sources(crossline-user-interface PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    modules/crossline_ui.cppm
)